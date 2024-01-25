#pragma once

#ifndef __DEATH_METHODS_H
#define __DEATH_METHODS_H

#include "InputPrompt.hpp"

class DeathMethods {
protected:
public:
    void link(CCObject*);
    void unlink(CCObject* obj);
    void showMenu(CCObject* obj);

private:
};

#endif