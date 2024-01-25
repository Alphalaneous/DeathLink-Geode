#include "DeathLinkMenu.h"
#include "DeathMethods.h"
#include <Geode/Geode.hpp>
#include "GlobalVars.h"

void DeathLinkMenu::setup() {
    
    ButtonSprite* linkSprite = ButtonSprite::create("Link", 100, true, "goldFont.fnt", "GJ_button_01.png", .0f, 1.f);
    ButtonSprite* unlinkSprite = ButtonSprite::create("Unlink", 100, true, "goldFont.fnt", "GJ_button_01.png", .0f, 1.f);

    CCMenuItemSpriteExtra* linkButton = CCMenuItemSpriteExtra::create(linkSprite, this, menu_selector(DeathMethods::link));
    CCMenuItemSpriteExtra* unlinkButton = CCMenuItemSpriteExtra::create(unlinkSprite, this, menu_selector(DeathMethods::unlink));

    linkButton->setPosition({0, 5});
    unlinkButton->setPosition({0, -35});

    CCMenu* buttonMenu = CCMenu::create();

    buttonMenu->addChild(linkButton);
    buttonMenu->addChild(unlinkButton);

    m_mainLayer->addChild(buttonMenu);

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    TextArea* linkedArea = TextArea::create("", "chatFont.fnt", 0.4f, 170, {}, 12, false);
    //linkedArea->setAnchorPoint({0.5, 1});
    linkedArea->setTag(16283);
    
    if(GlobalVars::getSharedInstance()->linkedTo != ""){
        linkedArea->setString(fmt::format("Linked to <cg>{}</c>", GlobalVars::getSharedInstance()->linkedTo).c_str());
    }
    else{
        linkedArea->setString("Linked to <cr>nobody</c>");
    }

    linkedArea->setContentSize({164, linkedArea->getContentSize().height});
    linkedArea->setPosition({winSize.width/2, winSize.height/2 - 62});
    this->m_mainLayer->addChild(linkedArea);

    setTouchEnabled(true);
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