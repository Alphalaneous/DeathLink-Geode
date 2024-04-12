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

	
		auto buttonSprite = CircleButtonSprite::createWithSprite("link.png"_spr, 1, CircleBaseColor::Green, CircleBaseSize::MediumAlt);

		auto button = CCMenuItemSpriteExtra::create(buttonSprite, this,
			menu_selector(MyMenuLayer::showDeathLinkMenu));

		button->setID("deathlink-button"_spr);

		auto winSize = CCDirector::sharedDirector()->getWinSize();
		
		if(auto menu = this->getChildByID("bottom-menu")){
			menu->addChild(button);
			menu->updateLayout();
		}

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

			if(GlobalVars::getSharedInstance()->lobbyID != ""){
				matjson::Value object;
				object.set("status", "death");

				ServerListener::sendMessage(object.dump());	
			}
		}

	}
};


$execute{
    ServerListener::connectAsync();
}
