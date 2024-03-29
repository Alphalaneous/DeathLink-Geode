#pragma once

#ifndef __MY_INPUT_NODE_H
#define __MY_INPUT_NODE_H

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MyInputNode : public cocos2d::CCNode {
    protected:
        cocos2d::extension::CCScale9Sprite* m_pBG;
        CCTextInputNode* m_pInput;

        bool init(float _w, float _h, const char* _phtxt, const char* _fnt, const std::string& _awc, int _cc) {
            this->m_pBG = cocos2d::extension::CCScale9Sprite::create(
                "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
            );

            this->m_pBG->setScale(.5f);
            this->m_pBG->setColor({ 0, 0, 0 });
            this->m_pBG->setOpacity(75);
            this->m_pBG->setContentSize({ _w * 2, _h * 2 });

            this->addChild(this->m_pBG);

            this->m_pInput = CCTextInputNode::create(_w - 10.0f, 60.0f, _phtxt, 3, _fnt);

            this->m_pInput->setLabelPlaceholderColor({ 150, 150, 150 });
            this->m_pInput->setLabelPlaceholderScale(.75f);
            this->m_pInput->setMaxLabelScale(.8f);
            this->m_pInput->setMaxLabelLength(_cc);
            if (_awc.length())
                this->m_pInput->setAllowedChars(_awc);

            this->addChild(this->m_pInput);

            return true;
        }
        bool init(float _w, const char* _phtxt, const char* _fnt, const std::string& _awc, int _cc) {
            return init(_w, 30.0f, _phtxt, _fnt, _awc, _cc);
        }
        bool initMulti(float _w, float _h, const char* _phtxt, const char* _fnt, const std::string& _awc, int _cc) {
            auto ret = init(_w, _h, _phtxt, _fnt, _awc, _cc);

            //this->m_pInput->setLineLength(_w);

            return ret;
        }

    public:
        static MyInputNode* create(
            float width,
            const char* placeholder,
            const char* fontFile,
            const std::string& filter,
            int limit
        ) {
            auto pRet = new MyInputNode();

            if (pRet && pRet->init(width, placeholder, fontFile, filter, limit)) {
                pRet->autorelease();
                return pRet;
            }

            CC_SAFE_DELETE(pRet);
            return nullptr;
        }
        static MyInputNode* create(
            float width,
            const char* placeholder,
            const std::string& filter,
            int limit
        ) {
            return create(width, placeholder, "bigFont.fnt", filter, limit);
        }
        static MyInputNode* create(
            float width,
            const char* placeholder,
            const std::string& filter
        ) {
            return create(width, placeholder, "bigFont.fnt", filter, 69);
        }
        static MyInputNode* create(
            float width,
            const char* placeholder,
            const char* fontFile
        ) {
            return create(width, placeholder, fontFile, "", 69);
        }
        static MyInputNode* create(
            float width,
            const char* placeholder
        ) {
            return create(width, placeholder, "bigFont.fnt");
        }

        // don't remember if this even works lol
        // the second float is height, rest are
        // the same as normal
        static MyInputNode* createMulti(float, float, const char*, const char*, const std::string &, int);
        static MyInputNode* createMulti(float, float, const char*, const char*, int);

        CCTextInputNode* getInputNode();
        cocos2d::extension::CCScale9Sprite* getBG();

        void setEnabled(bool);

        void setString(const char* _str) {
            this->m_pInput->m_textField->setString(_str);

            this->m_pInput->refreshLabel();
        }
        const char* getString() {
            return this->m_pInput->m_textField->getString();
        }
};

#endif