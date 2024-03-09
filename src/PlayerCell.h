#pragma once

#ifndef __PLAYERCELL_H
#define __PLAYERCELL_H

#include <Geode/Geode.hpp>

class PlayerCell : public CCLayer {


public:

    std::string username;
    int accountID;
    int userID;
    bool isHost;

    bool init(std::string username, int accountID, int userID, bool isHost){

        this->username = username;
        this->accountID = accountID;
        this->userID = userID;
        this->isHost = isHost;

        CCLabelBMFont* memberName = CCLabelBMFont::create(username.c_str(), "bigFont.fnt");
        
        if(isHost){
            memberName->setColor({100, 255, 100});
        }

        std::string upperUsername;
        std::string upperAccountUsername;

        for(auto elem : username){
            upperUsername += std::toupper(elem);
        }

        for(auto elem : std::string(GJAccountManager::sharedState()->m_username)){
            upperAccountUsername += std::toupper(elem);
        }

        if(upperUsername == "XANII"){
            memberName->setColor({160, 28, 255});
        }

        CCMenuItemSpriteExtra* profileButton = CCMenuItemSpriteExtra::create(memberName, this, menu_selector(PlayerCell::showProfilePage));
        profileButton->setTag(accountID);

        float scale = 0.6f;

        profileButton->setAnchorPoint({0, 0.5});
        profileButton->setPosition({10/scale, 15/scale});

        CCMenu* buttonMenu = CCMenu::create();
        buttonMenu->setContentSize({200/scale, 30/scale});
        buttonMenu->ignoreAnchorPointForPosition(false);
        buttonMenu->setAnchorPoint({0, 0});
        buttonMenu->setPosition({0, 0.5});
        buttonMenu->setScale(scale);

        buttonMenu->addChild(profileButton);

        CCSprite* kickSprite = CCSprite::createWithSpriteFrameName("accountBtn_removeFriend_001.png");

        CCMenuItemSpriteExtra* kickButton = CCMenuItemSpriteExtra::create(kickSprite, this, menu_selector(PlayerCell::kickUser));
        kickButton->setAnchorPoint({0.5, 0.5});
        kickButton->setPosition({250/scale - 20/scale, 15/scale});

        

        if(upperUsername != upperAccountUsername && GlobalVars::getSharedInstance()->isHost){
            buttonMenu->addChild(kickButton);
        }

        this->addChild(buttonMenu);
        this->setPosition(this->getPositionX(), this->getPositionY()-30);
        return true;
    }

    void showProfilePage(CCObject* obj){

        ProfilePage* profilePage = ProfilePage::create(accountID, false);
        profilePage->show();
    }


    void kickUser(CCObject* obj){
        auto alertLayer = createQuickPopup("Kick Player?", "Are you sure you would like to kick " + username + "?", "Cancel", "Kick",
            [this](FLAlertLayer*, bool button2){
                if(button2){

                    matjson::Value object;
                    object.set("status", "kick");
                    object.set("account_id", accountID);

                    ServerListener::sendMessage(object.dump());	
                }
        });
    }

    static PlayerCell* create(std::string username, int accountID, int userID, bool isHost){
     
        auto ret = new PlayerCell();
        if (ret && ret->init(username, accountID, userID, isHost)) {
            ret->autorelease();
        } else {
            delete ret;
            ret = nullptr;
        }
        return ret;
    }
};

#endif
