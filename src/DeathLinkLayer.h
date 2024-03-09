#pragma once

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class DeathLinkLayer : public cocos2d::CCLayer {
protected:
    virtual bool init();
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
public:
    GJListLayer* m_list = nullptr;
    void reloadList();
    static DeathLinkLayer* create();
    static cocos2d::CCScene* scene();
    void showHostError(float dt);
    void showServerError(float dt);
    void copyCode(CCObject* obj);
    void leaveLobby(CCObject* obj);

};