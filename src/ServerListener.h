#pragma once

#ifndef __SERVERLISTENER_H
#define __SERVERLISTENER_H

#include <Geode/Geode.hpp>

using namespace cocos2d;

class ServerListener {
protected:
public:

    static void connectAsync();
    static void connect();
    static void open();
    static void onMessage(std::string message);
    static void sendMessage(std::string message);
private:
};

#endif