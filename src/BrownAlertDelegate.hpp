#pragma once

#ifndef __BROWN_ALERT_DELEGATE_H
#define __BROWN_ALERT_DELEGATE_H

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BrownAlertDelegate : public FLAlertLayer {
    protected:
        // layer size is stored here
        cocos2d::CCSize m_pLrSize;
        cocos2d::extension::CCScale9Sprite* m_pBGSprite;

        bool init(float _w, float _h, const char* _spr = "GJ_square01.png", const char* _title = "");

        virtual void setup() = 0;
        virtual void keyDown(cocos2d::enumKeyCodes key) override {
            if (key == cocos2d::enumKeyCodes::KEY_Escape)
                return onClose(nullptr);
            if (key == cocos2d::enumKeyCodes::KEY_Space)
                return;

            return FLAlertLayer::keyDown(key);
        }
        
    public:
        virtual void onClose(cocos2d::CCObject*) {
            this->setKeyboardEnabled(false);
            this->removeFromParentAndCleanup(true);
        }
};

#endif
