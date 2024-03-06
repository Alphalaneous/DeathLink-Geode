#pragma once

#ifndef __GLOBALVARS_H
#define __GLOBALVARS_H

#include <Geode/Geode.hpp>

class GlobalVars {


protected:
    static GlobalVars* instance;
public:

    bool isOpen = false;
    std::string lobbyID = "";
    matjson::Array members;
    bool showHostError = false;
    bool showServerError = false;
    bool showKickedError = false;
    bool hasDeathQueued = false;
    bool isHost = false;

    static GlobalVars* getSharedInstance(){

        if (!instance) {
            instance = new GlobalVars();
        };
        return instance;
    }

};

#endif
