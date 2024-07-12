#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {

	bool init() {

		if (!MenuLayer::init()) return false;

	
		auto buttonSprite = CircleButtonSprite::createWithSprite("link.png"_spr, 1, CircleBaseColor::Green, CircleBaseSize::MediumAlt);

		auto button = CCMenuItemSpriteExtra::create(buttonSprite, this,
			menu_selector(MyMenuLayer::showDeathLinkMenu));

		button->setID("deathlink-button"_spr);
		
		if(auto menu = this->getChildByID("bottom-menu")){
			menu->addChild(button);
			menu->updateLayout();
		}

		return true;
	} 

	void showDeathLinkMenu(CCObject* obj){

		auto alertLayer = FLAlertLayer::create(nullptr, "DeathLink", "DeathLink is discontinued, it will eventually be added to Globed.", "Okay", nullptr, 250);
		alertLayer->show();
	}
};

