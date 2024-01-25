#include "InputPrompt.hpp"

bool InputPrompt::init(const char* title, const char* inputText, InputResult resFunc, const char* applyText) {
    
    if (!BrownAlertDelegate::init(250.0f, 140.0f, "GJ_square05.png", title))
        return false;

    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

    this->m_pInput = MyInputNode::create(160.0f, inputText, "bigFont.fnt");
    this->m_pInput->setPosition(winSize / 2);
    this->m_mainLayer->addChild(this->m_pInput);
    this->m_pResultFunc = resFunc;

    auto button = CCMenuItemSpriteExtra::create(
        ButtonSprite::create(
            applyText, 0, 0, "goldFont.fnt", "GJ_button_01.png", 0, .8f
        ),
        this,
        (cocos2d::SEL_MenuHandler)&InputPrompt::onApply
    );
    button->setPosition(0.0f, -this->m_pLrSize.height / 2 + 25.0f);

    this->m_buttonMenu->addChild(button);

    this->m_pTrashBtn = CCMenuItemSpriteExtra::create(
        cocos2d::CCSprite::createWithSpriteFrameName("GJ_resetBtn_001.png"),
        this,
        (cocos2d::SEL_MenuHandler)&InputPrompt::onTrash
    );
    this->m_pTrashBtn->setPosition(
        cocos2d::CCPoint{ 0, 0 } - this->m_pLrSize / 2 + cocos2d::CCPoint{ 25.0f, 25.0f }
    );
    this->m_pTrashBtn->setVisible(false);

    this->m_buttonMenu->addChild(this->m_pTrashBtn);

    return true;
}
