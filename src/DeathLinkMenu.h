#pragma once

#ifndef __DEATHLINKMENU_HPP
#define __DEATHLINKMENU_HPP

#include "BrownAlertDelegate.hpp"

class DeathLinkMenu : public BrownAlertDelegate {
    protected:
        virtual void setup();
        static DeathLinkMenu* instance;

        ~DeathLinkMenu();

    public:
        static DeathLinkMenu* create();
        static DeathLinkMenu* get();

};

#endif