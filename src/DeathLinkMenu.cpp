#include "ServerListener.h"
#include "GlobalVars.h"
#include "Utils.hpp"
#include "DeathLinkMenu.h"
#include <algorithm>

void DeathLinkMenu::setup() {
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    m_linkSprite = CCSprite::createWithSpriteFrameName("gj_linkBtn_001.png");
    m_unlinkSprite = CCSprite::createWithSpriteFrameName("gj_linkBtnOff_001.png");

    m_linkButton = CCMenuItemSpriteExtra::create(m_linkSprite, this, menu_selector(DeathLinkMenu::onLink));
    m_unlinkButton = CCMenuItemSpriteExtra::create(m_unlinkSprite, this, menu_selector(DeathLinkMenu::onUnlink));
    log::info("default color: {}", m_linkSprite->getColor());
    
    m_linkedTo = GlobalVars::getSharedInstance()->linkedTo;
    bool isLinked = m_linkedTo != "";
    log::info("linkedTo is: {} and isLinked is: {}", m_linkedTo.c_str(), isLinked);

    updateButtonStatus(isLinked);

    m_userInput = MyInputNode::create(160.0f, "Username", "bigFont.fnt");
    m_userInput->setString(m_linkedTo.c_str());
    m_userInput->setPosition(winSize.width / 2, (winSize.height / 2) + 5);

    CCMenu* buttonMenu = CCMenu::create();

    buttonMenu->addChild(m_unlinkButton);
    buttonMenu->addChild(m_linkButton);
    buttonMenu->setLayout(AxisLayout::create()->setGap(20));
    buttonMenu->setPosition({winSize.width / 2, (winSize.height / 2) - 36});

    m_mainLayer->addChild(m_userInput);
    m_mainLayer->addChild(buttonMenu);


    m_linkedArea = TextArea::create("", "chatFont.fnt", 0.4f, 170, {}, 12, false);
    //linkedArea->setAnchorPoint({0.5, 1});
    
    if(GlobalVars::getSharedInstance()->linkedTo != ""){
        m_linkedArea->setString(fmt::format("Linked to <cg>{}</c>", GlobalVars::getSharedInstance()->linkedTo).c_str());
    }
    else{
        m_linkedArea->setString("Linked to <cr>nobody</c>");
    }

    m_linkedArea->setContentSize({164, m_linkedArea->getContentSize().height});
    m_linkedArea->setPosition({winSize.width/2, winSize.height/2 - 62});
    this->m_mainLayer->addChild(m_linkedArea);

    setTouchEnabled(true);
}

void DeathLinkMenu::onLink(CCObject* sender) {
    std::string input = this->m_userInput->getString();

    Utils::ltrim(input);
    Utils::rtrim(input);

    std::string lower;

    for(auto elem : input)
       lower += std::tolower(elem);
    std::string lowerUsername;
    for(auto elem : std::string(GJAccountManager::sharedState()->m_username))
       lowerUsername += std::tolower(elem);
    if(lower == lowerUsername){
        auto alertLayer = FLAlertLayer::create(nullptr, "Oops", "You can't link to yourself silly :P", "Okay", nullptr, 250);
        alertLayer->show();
        return;
    }
    if(lower == m_linkedTo) {
        auto alertLayer = FLAlertLayer::create(nullptr, "Oops", "You're already linked to this person :P", "Okay", nullptr, 250);
        alertLayer->show();
        return;
    }

    if (Utils::containsCharNotInList(lower)){
        auto alertLayer = FLAlertLayer::create(nullptr, "Oops", "Those characters aren't allowed :(", "Okay", nullptr, 250);
        alertLayer->show();
        return;
    }

    if(input.length() > 0){
        log::info("link: {}", std::string(m_userInput->getString()));
        ServerListener::sendMessage("link:" + std::string(m_userInput->getString()));
        GlobalVars::getSharedInstance()->disconnected = false;
        GlobalVars::getSharedInstance()->linkedTo = input;
        m_linkedTo = input;
        auto alertLayer = FLAlertLayer::create(nullptr, "Success", fmt::format("You have been linked to <cg>{}</c>!", input), "Okay", nullptr, 250);
        alertLayer->show();

        m_linkedArea->setString(fmt::format("Linked to <cg>{}</c>", input).c_str());
        m_linkedArea->setContentSize({164, m_linkedArea->getContentSize().height});

    }
    DeathLinkMenu::updateButtonStatus(GlobalVars::getSharedInstance()->linkedTo != "");
}

void DeathLinkMenu::onUnlink(CCObject* obj) {

    GlobalVars::getSharedInstance()->disconnected = true;

    auto alertLayer = FLAlertLayer::create(nullptr, "Success", "You have been <cr>unlinked</c>.", "Okay", nullptr, 250);

    alertLayer->show();

    GlobalVars::getSharedInstance()->linkedTo = "";
    m_linkedTo = "";
    

    m_linkedArea->setString("linked to <cr>nobody</c>");
    m_linkedArea->setContentSize({164, m_linkedArea->getContentSize().height});
    
    DeathLinkMenu::updateButtonStatus(GlobalVars::getSharedInstance()->linkedTo != "");
}

void DeathLinkMenu::onDeathlink(CCObject* obj) {

    if(!GlobalVars::getSharedInstance()->isOpen){
        ServerListener::connectAsync();
    }

    DeathLinkMenu* menu = DeathLinkMenu::create();
    menu->show();
  
}

void DeathLinkMenu::updateButtonStatus(bool isLinked) {
    if(!isLinked){
        m_unlinkSprite->setColor({ 100, 100, 100 });
        m_unlinkButton->setEnabled(false);
        return;
    }
    m_unlinkSprite->setColor({ 255, 255, 255 });
    m_unlinkButton->setEnabled(true);
}


DeathLinkMenu* DeathLinkMenu::create() {
    auto pRet = new DeathLinkMenu();
    DeathLinkMenu::instance = pRet;
    if (pRet && pRet->init(180, 140, "GJ_square01.png", "DeathLink")) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}



DeathLinkMenu* DeathLinkMenu::instance = nullptr;

DeathLinkMenu* DeathLinkMenu::get(){
    return DeathLinkMenu::instance;
}

DeathLinkMenu::~DeathLinkMenu(){
    DeathLinkMenu::instance = nullptr;
}