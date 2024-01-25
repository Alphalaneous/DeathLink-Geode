#pragma once

#ifndef __INPUT_PROMPT_H
#define __INPUT_PROMPT_H

#include <Geode/Geode.hpp>
#include "BrownAlertDelegate.hpp"
#include "MyInputNode.hpp"

using namespace geode::prelude;

class InputPrompt : public BrownAlertDelegate {
    public:
        using InputResult = std::function<void(const char*)>;
    protected:
        MyInputNode* m_pInput;
        InputResult m_pResultFunc;
        CCMenuItemSpriteExtra* m_pTrashBtn;
        bool m_bApplyOnEsc = false;

        inline void setup() override {};
        void onApply(cocos2d::CCObject*) {
            this->m_pResultFunc(this->m_pInput->getString());

            std::string input = this->m_pInput->getString();

            ltrim(input);
            rtrim(input);

            if(input.length() > 0){
                this->onClose(nullptr);
            }
        }
        void onTrash(cocos2d::CCObject*) {
            this->m_pInput->setString("");

            if (!this->m_bApplyOnEsc)
                this->m_pResultFunc("");

            this->onClose(nullptr);
        }
        void onClose(cocos2d::CCObject*) override
        {
            if (this->m_bApplyOnEsc)
                this->m_pResultFunc(this->m_pInput->getString());

            BrownAlertDelegate::onClose(nullptr);
        }

    
        inline void ltrim(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
        }

        inline void rtrim(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(), s.end());
        }

        bool init(const char* title, const char* inputText, InputResult resFunc, const char* applyText);

    public:
        inline static InputPrompt* create(
            const char* title,
            const char* inputText,
            InputResult resFunc,
            const char* applyText = "Apply"
        ) {
            auto pRet = new InputPrompt();

            if (pRet && pRet->init(title, inputText, resFunc, applyText)) {
                pRet->autorelease();
                return pRet;
            }

            CC_SAFE_DELETE(pRet);
            return nullptr;
        }

        inline InputPrompt* setApplyOnEsc(bool on) {
            this->m_bApplyOnEsc = on; return this;
        }

        inline InputPrompt* setTrashButton(bool on) {
            this->m_pTrashBtn->setVisible(on); return this;
        }

        inline InputPrompt* setCallback(InputResult resFunc) {
            this->m_pResultFunc = resFunc; return this;
        }

        inline InputPrompt* setFilter(std::string const& filter) {
            this->m_pInput->getInputNode()->setAllowedChars(filter);
            return this;
        }

        inline MyInputNode* getInputNode() {
            return m_pInput;
        }
};
#endif