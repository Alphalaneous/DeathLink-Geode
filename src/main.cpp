#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include "DeathLinkChoiceMenu.h"
#include "ServerListener.h"
#include "DeathLinkLayer.h"
#include "GlobalVars.h"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {

	bool init() {

		if (!MenuLayer::init()) return false;

	
		auto buttonSprite = CCSprite::createWithSpriteFrameName("gj_linkBtn_001.png");

		auto button = CCMenuItemSpriteExtra::create(buttonSprite, this,
			menu_selector(MyMenuLayer::showDeathLinkMenu));

		button->setID("deathlink-button"_spr);

		auto winSize = CCDirector::sharedDirector()->getWinSize();

		button->setPosition({ 66, 92 });

		auto menu = CCMenu::create();
		menu->addChild(button);
		menu->setID("deathlink-menu"_spr);

		menu->setPosition({ 0,0 });

		this->addChild(menu);

		this->scheduleUpdate();
		this->scheduleOnce(schedule_selector(MyMenuLayer::delayChecks), 1);

		return true;
	} 

	void delayChecks(float dt){
		if(GlobalVars::getSharedInstance()->showHostError){
			auto alertLayer = FLAlertLayer::create(nullptr, "DeathLink", "The host has disconnected.", "Okay", nullptr, 250);
            alertLayer->show();
			GlobalVars::getSharedInstance()->showHostError = false;
		}

		if(GlobalVars::getSharedInstance()->showServerError){
			auto alertLayer = FLAlertLayer::create(nullptr, "DeathLink", "The server has disconnected.", "Okay", nullptr, 250);
            alertLayer->show();
			GlobalVars::getSharedInstance()->showServerError = false;
		}

		if(GlobalVars::getSharedInstance()->showKickedError){
			auto alertLayer = FLAlertLayer::create(nullptr, "DeathLink", "The host has kicked you.", "Okay", nullptr, 250);
            alertLayer->show();
			GlobalVars::getSharedInstance()->showKickedError = false;
		}
	}

	void showDeathLinkMenu(CCObject* obj){

		if(std::string(GJAccountManager::sharedState()->m_username) == ""){
            auto alertLayer = FLAlertLayer::create(nullptr, "DeathLink", "You must be logged into GD to play. If you are, restart GD.", "Okay", nullptr, 250);
            alertLayer->show();
            return;
        }

		if(!GlobalVars::getSharedInstance()->isOpen){
    		ServerListener::connectAsync();
		}

		if(GlobalVars::getSharedInstance()->lobbyID != ""){
			CCScene* scene = DeathLinkLayer::scene();
			auto transition = CCTransitionFade::create(0.5f, scene);
			CCDirector::sharedDirector()->pushScene(transition);
		}
		else{
			DeathLinkChoiceMenu* dlcm = DeathLinkChoiceMenu::create();
			dlcm->show();
		}
	}

};

class $modify(PlayerObject){
	void playerDestroyed(bool param_1){

		PlayerObject::playerDestroyed(param_1);

		if(!GlobalVars::getSharedInstance()->hasDeathQueued){
			matjson::Value object;
			object.set("status", "death");

			ServerListener::sendMessage(object.dump());	
		}

		GlobalVars::getSharedInstance()->hasDeathQueued = false;
	}
};


$execute{
    ServerListener::connectAsync();
}
