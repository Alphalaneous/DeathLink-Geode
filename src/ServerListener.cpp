
#include "easywsclient.hpp"
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string>
#include <memory>
#include "ServerListener.h"
#include <Geode/Geode.hpp>
#include <thread>
#include "GlobalVars.h"

using namespace cocos2d;

using namespace easywsclient;

std::unique_ptr<WebSocket> ws(nullptr);

void ServerListener::connectAsync() {
    std::thread t = std::thread(connect);
    t.detach();
}


void ServerListener::connect(){

    open();

    using easywsclient::WebSocket;

#ifdef _WIN32
    INT rc;
    WSADATA wsaData;

    rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc) {
        return;
    }
#endif

    if (ws) {

        GlobalVars::getSharedInstance()->isOpen = true;
        ws->send("name:" + std::string(GJAccountManager::sharedState()->m_username));

        while (ws->getReadyState() != WebSocket::CLOSED) {
            WebSocket::pointer wsp = &*ws; // a unique_ptr cannot be copied into a lambda
            ws->poll();
            ws->dispatch([wsp](const std::string& message) {
                onMessage(message);
                geode::log::info("{}", message);

                });
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    //closed
    GlobalVars::getSharedInstance()->isOpen = false;
    ws.release();

#ifdef _WIN32
    WSACleanup();
#endif

}


void ServerListener::onMessage(std::string message) {
    geode::Loader::get()->queueInMainThread([message]() {
        if(!GlobalVars::getSharedInstance()->disconnected){
            if (message == "dead") {

                
                PlayLayer* playLayer = PlayLayer::get();

                if (playLayer != nullptr) {

                    playLayer->PlayLayer::destroyPlayer(playLayer->m_player1, nullptr);
                }
            }
            if(message.substr(0, 4) == "say:") {
                
                auto alertLayer = FLAlertLayer::create(nullptr, "DeathLink", message.substr(5), "Okay", nullptr, 250);

                alertLayer->show();

            }
            if(message == "tooManyRequests"){
                auto alertLayer = FLAlertLayer::create(nullptr, "Slow Down!", "You have been automatically disconnected for sending too many requests to the DeathLink servers", "Okay", nullptr, 250);

                alertLayer->show();
            }
        }
    });
}

void ServerListener::sendMessage(std::string message) {
    if(GlobalVars::getSharedInstance()->isOpen){
        ws->send(message);
    }
}

void ServerListener::open() {

    ws.release();
    ws.reset(WebSocket::from_url("ws://164.152.25.111:4732"));

}