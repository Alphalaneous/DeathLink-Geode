#include "DeathLinkLayer.h"
#include "GlobalVars.h"
#include "ServerListener.h"
#include "PlayerCell.h"

DeathLinkLayer* DeathLinkLayer::create() {
    auto ret = new DeathLinkLayer();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool DeathLinkLayer::init() {

    auto backgroundSprite = CCSprite::create("GJ_gradientBG.png");
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto size = backgroundSprite->getContentSize(); 
    
    backgroundSprite->setScaleX(winSize.width / size.width);
    backgroundSprite->setScaleY(winSize.height / size.height);
    backgroundSprite->setID("background"_spr);
    
    backgroundSprite->setAnchorPoint({0, 0});
    backgroundSprite->setColor({0, 40, 50});
    
    backgroundSprite->setZOrder(-2);
    addChild(backgroundSprite);

    auto backBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"),
        this,
        menu_selector(DeathLinkLayer::onBack)
    );
    backBtn->setID("exit-button"_spr);

    auto menuBack = CCMenu::create();
    menuBack->addChild(backBtn);
    menuBack->setPosition({25, winSize.height - 25});
    menuBack->setID("exit-menu"_spr);

    CCSprite* m_cornerBL = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    m_cornerBL->setPosition({0,0});
    m_cornerBL->setAnchorPoint({0,0});
    addChild(m_cornerBL, -1);
    m_cornerBL->setID("corner-bl"_spr);

    CCSprite* m_cornerTL = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    m_cornerTL->setPosition({0,winSize.height});
    m_cornerTL->setAnchorPoint({0,1});
    m_cornerTL->setFlipY(true);
    m_cornerTL->setID("corner-ul"_spr);
    addChild(m_cornerTL, -1);

    CCSprite* m_cornerTR = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    m_cornerTR->setPosition({winSize.width,winSize.height});
    m_cornerTR->setAnchorPoint({1,1});
    m_cornerTR->setFlipX(true);
    m_cornerTR->setFlipY(true);
    m_cornerTR->setID("corner-ur"_spr);
    addChild(m_cornerTR, -1);

    CCSprite* m_cornerBR = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    m_cornerBR->setPosition({winSize.width,0});
    m_cornerBR->setAnchorPoint({1,0});
    m_cornerBR->setFlipX(true);
    m_cornerBR->setID("corner-br"_spr);
    addChild(m_cornerBR, -1);

    addChild(menuBack);

    CCSprite* titleSprite = CCSprite::create("deathlink-logo.png"_spr);

    titleSprite->setPosition({winSize.width/2, winSize.height-35});
    titleSprite->setScale(0.75f);
    titleSprite->setZOrder(1);
    addChild(titleSprite);


    CCMenu* buttonsMenu = CCMenu::create();
    buttonsMenu->ignoreAnchorPointForPosition(false);

    ButtonSprite* copyLobbyIDSprite = ButtonSprite::create("Copy Code", 100, true, "goldFont.fnt", "GJ_button_01.png", 30, 1);
    
    CCMenuItemSpriteExtra* copyLobbyIDButton = CCMenuItemSpriteExtra::create(copyLobbyIDSprite, this, menu_selector(DeathLinkLayer::copyCode));
    copyLobbyIDButton->setPosition({winSize.width/2-65, 25});

    buttonsMenu->addChild(copyLobbyIDButton);

    ButtonSprite* leaveSprite = ButtonSprite::create("Leave Lobby", 100, true, "goldFont.fnt", "GJ_button_01.png", 30, 1);
    
    CCMenuItemSpriteExtra* leaveButton = CCMenuItemSpriteExtra::create(leaveSprite, this, menu_selector(DeathLinkLayer::leaveLobby));
    leaveButton->setPosition({winSize.width/2+65, 25});

    buttonsMenu->addChild(leaveButton);

    reloadList();

    addChild(buttonsMenu);

    setKeypadEnabled(true);

    return true;
}

void DeathLinkLayer::reloadList(){

    CCArray* arr = CCArray::create();

    float height = 200.0f;
    float width = 250.0f;

    matjson::Array members = GlobalVars::getSharedInstance()->members;

    for (int i = 0; i < members.size(); i++){

        matjson::Object memberObject = members.at(i).as_object();

        std::string username = memberObject["username"].as_string();
        int accountID = memberObject["account_id"].as_int();
        int userID = memberObject["user_id"].as_int();
        bool isHost = memberObject["is_host"].as_bool();

        GameLevelManager::sharedState()->storeUserName(userID, accountID, username.c_str());

        PlayerCell* cell = PlayerCell::create(username, accountID, userID, isHost);
       
        arr->addObject(cell);
    }

    float scroll = height - members.size()*30;


    if (m_list) {
        scroll = m_list->m_listView->m_tableView->m_contentLayer->getPositionY();
        m_list->removeFromParent();
    }

    geode::ListView* listView = geode::ListView::create(arr, 30, width, height);

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    m_list = GJListLayer::create(listView, "", {191, 114, 62, 255}, width, height, 1);
    m_list->setID("list-layer"_spr);
    m_list->setAnchorPoint({0.5, 0.5});
    m_list->ignoreAnchorPointForPosition(false);
    m_list->setPosition(winSize.width/2, winSize.height/2 + 5);


    if(listView->m_tableView->m_contentLayer->getContentSize().height > height){
        listView->m_tableView->m_contentLayer->setPosition({0.0f, scroll});
    }

    addChild(m_list);
}

void DeathLinkLayer::keyBackClicked() {
    CCScene* scene = MenuLayer::scene(false);
    auto transition = CCTransitionFade::create(0.5f, scene);
    CCDirector::sharedDirector()->pushScene(transition);
}


void DeathLinkLayer::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* DeathLinkLayer::scene() {
    auto layer = DeathLinkLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}



void DeathLinkLayer::showHostError(float dt){

    auto alertLayer = FLAlertLayer::create(nullptr, "Disconnected", "The host has disconnected.", "Okay", nullptr, 250);
    alertLayer->show();

}

void DeathLinkLayer::showServerError(float dt){

    auto alertLayer = FLAlertLayer::create(nullptr, "Disconnected", "The server has disconnected.", "Okay", nullptr, 250);
    alertLayer->show();

}

void DeathLinkLayer::copyCode(CCObject* obj) {

    std::string requester = GlobalVars::getSharedInstance()->lobbyID;
    clipboard::write(requester);

}

void DeathLinkLayer::leaveLobby(CCObject* obj) {

    std::string desc = "Are you sure you want to leave the lobby?";

    if(GlobalVars::getSharedInstance()->isHost){

        desc = "You are the host, if you leave, everyone else will be disconnected.";

        GlobalVars::getSharedInstance()->isHost = false;
    }

    auto alertLayer = createQuickPopup("Leave Lobby?", desc.c_str(), "Cancel", "Okay",
    [](FLAlertLayer*, bool button2){
        if(button2){

            GlobalVars::getSharedInstance()->lobbyID = "";
            
            matjson::Value object;
            object.set("status", "disconnect_lobby");

            ServerListener::sendMessage(object.dump());	

            CCScene* scene = MenuLayer::scene(false);
            auto transition = CCTransitionFade::create(0.5f, scene);
            CCDirector::sharedDirector()->pushScene(transition);

        }
    });
}