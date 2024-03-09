#pragma once

#ifndef __DEATHLINK_CHOICE_HPP
#define __DEATHLINK_CHOICE_HPP

#include "BrownAlertDelegate.h"

class DeathLinkChoiceMenu : public BrownAlertDelegate {
    protected:
        
        virtual void setup();
        
    public:
        static DeathLinkChoiceMenu* create();
        void onJoin(CCObject*);
        void onCreate(CCObject*);
};

#endif