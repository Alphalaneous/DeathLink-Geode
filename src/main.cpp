#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include "DeathMethods.h"
#include "ServerListener.h"
#include "GlobalVars.h"

using namespace geode::prelude;

class $modify(MenuLayer) {

	bool init() {

		if (!MenuLayer::init()) return false;

		auto buttonSprite = CCSprite::createWithSpriteFrameName("gj_linkBtn_001.png");

		auto button = CCMenuItemSpriteExtra::create(buttonSprite, this,
			menu_selector(DeathMethods::showMenu));

		auto winSize = CCDirector::sharedDirector()->getWinSize();

		button->setPosition({ 66, 92 });

		auto menu = CCMenu::create();
		menu->addChild(button);

		menu->setPosition({ 0,0 });

		this->addChild(menu);

		return true;
	} 
};

class $modify(PlayerObject){
	void playerDestroyed(bool param_1){

		PlayerObject::playerDestroyed(param_1);

    	ServerListener::sendMessage("dead");
	}
};


$execute{
    ServerListener::connectAsync();
}
