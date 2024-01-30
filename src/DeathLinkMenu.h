#pragma once

#ifndef __DEATHLINKMENU_HPP
#define __DEATHLINKMENU_HPP

#include "BrownAlertDelegate.hpp"
#include "MyInputNode.hpp"

class DeathLinkMenu : public BrownAlertDelegate {
    protected:
        virtual void setup();
        static DeathLinkMenu* instance;

        ~DeathLinkMenu();

    public:
        static DeathLinkMenu* create();
        static DeathLinkMenu* get();
        void updateButtonStatus(bool isLinked);
        void onLink(CCObject* sender);
        void onUnlink(CCObject* sender);
        void onDeathlink(CCObject* obj);

        std::string m_linkedTo;
        cocos2d::CCSprite* m_linkSprite;
        cocos2d::CCSprite* m_unlinkSprite;
        CCMenuItemSpriteExtra* m_linkButton;
        CCMenuItemSpriteExtra* m_unlinkButton;
        MyInputNode* m_userInput;
        TextArea* m_linkedArea;
};

#endif