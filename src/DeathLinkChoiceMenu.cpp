#include "DeathLinkChoiceMenu.h"
#include "MyInputNode.h"
#include "ServerListener.h"
#include "Utils.h"
#include "DeathLinkLayer.h"
#include <Geode/utils/web.hpp>

void DeathLinkChoiceMenu::setup() {

    CCLabelBMFont* title = CCLabelBMFont::create("DeathLink", "bigFont.fnt");

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    title->setPosition({winSize.width/2, winSize.height/2+70});

    m_mainLayer->addChild(title);

    ButtonSprite* joinSprite = ButtonSprite::create("Join Lobby", 200, true, "goldFont.fnt", "GJ_button_01.png", 30, 1);

    ButtonSprite* hostSprite = ButtonSprite::create("Create Lobby", 200, true, "goldFont.fnt", "GJ_button_01.png", 30, 1);
    
    CCMenuItemSpriteExtra* joinButton = CCMenuItemSpriteExtra::create(joinSprite, this, menu_selector(DeathLinkChoiceMenu::onJoin));
    joinButton->setPosition({winSize.width/2, winSize.height/2 - 25});

    CCMenuItemSpriteExtra* hostButton = CCMenuItemSpriteExtra::create(hostSprite, this, menu_selector(DeathLinkChoiceMenu::onCreate));
    hostButton->setPosition({winSize.width/2, winSize.height/2 - 70});

    m_buttonMenu->addChild(joinButton);
    m_buttonMenu->addChild(hostButton);

    MyInputNode* lobbyIDInput = MyInputNode::create(200, "Lobby Code");
    
    lobbyIDInput->setID("lobby-id-input"_spr);

    lobbyIDInput->setPosition({winSize.width/2, winSize.height/2 + 25});

    m_buttonMenu->addChild(lobbyIDInput);

    setTouchEnabled(true);
}

void DeathLinkChoiceMenu::onJoin(CCObject* obj){
    MyInputNode* node = dynamic_cast<MyInputNode*>(this->getChildByIDRecursive("lobby-id-input"_spr));

    std::string str = node->getString();

    std::string upper;

    for(auto elem : str){
       upper += std::toupper(elem);
    }

    if(upper == "" || Utils::containsCharNotInList(upper) || upper.length() > 6){
        std::string reason = "";

        if(upper == "") reason = "Lobby Code is empty";
        if(Utils::containsCharNotInList(upper)) reason = "Contains Invalid Characters";
        if(upper.length() > 6) reason = "Above Character Limit";
        
        auto alertLayer = FLAlertLayer::create(nullptr, "Oops", fmt::format("Invalid Lobby Code!\n<cr>({})</c>", reason), "Okay", nullptr, 350);
        alertLayer->show();
        return;
    }

    if(upper.length() > 0){

        matjson::Value object;
        object.set("status", "connect_to_lobby");
        object.set("lobby_id", upper);

        ServerListener::sendMessage(object.dump());
    }
}

void DeathLinkChoiceMenu::onCreate(CCObject* obj){
    matjson::Value object;
    object.set("status", "host_lobby");

    ServerListener::sendMessage(object.dump());
}


DeathLinkChoiceMenu* DeathLinkChoiceMenu::create() {

    auto pRet = new DeathLinkChoiceMenu();

    if (pRet && pRet->init(250, 200, "GJ_square01.png", "")) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
};
