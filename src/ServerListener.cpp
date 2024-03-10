
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
#include "DeathLinkLayer.h"

using namespace cocos2d;

using namespace easywsclient;

std::unique_ptr<WebSocket> ws(nullptr);

void ServerListener::connectAsync() {
    std::thread t = std::thread(connect);
    t.detach();
}

bool fromDeathLinkLayer(){
    CCScene* currentScene = CCDirector::sharedDirector()->getRunningScene();

    DeathLinkLayer* deathLinkLayer = dynamic_cast<DeathLinkLayer*>(currentScene->getChildren()->objectAtIndex(0));

    if(deathLinkLayer){
        return true;
    }
    return false;
}

void leaveLobbyDisconnected(){
    CCScene* currentScene = CCDirector::sharedDirector()->getRunningScene();

    DeathLinkLayer* deathLinkLayer = dynamic_cast<DeathLinkLayer*>(currentScene->getChildren()->objectAtIndex(0));

    if(deathLinkLayer){
        CCScene* scene = MenuLayer::scene(false);
        auto transition = CCTransitionFade::create(0.5f, scene);
        CCDirector::sharedDirector()->pushScene(transition);
    }
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

        matjson::Value object;

    
        object.set("status", "connect");
        object.set("username", std::string(GJAccountManager::sharedState()->m_username));
        object.set("account_id", GJAccountManager::sharedState()->m_accountID);
        object.set("user_id", GameManager::get()->m_playerUserID.value());

        ws->send(object.dump());

        while (ws->getReadyState() != WebSocket::CLOSED) {
            WebSocket::pointer wsp = &*ws; 
            ws->poll();
            ws->dispatch([wsp](const std::string& message) {
                onMessage(message);
                //geode::log::info("{}", message);

                });
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    //closed
    GlobalVars::getSharedInstance()->isOpen = false;
    GlobalVars::getSharedInstance()->lobbyID = "";

    geode::Loader::get()->queueInMainThread([]() {
        if(fromDeathLinkLayer()){
            GlobalVars::getSharedInstance()->showServerError = true;
            leaveLobbyDisconnected();
        }
        else{
            PlayLayer* playLayer = PlayLayer::get();

            if (playLayer) {
                Notification* notification = Notification::create("The server has disconnected.", NotificationIcon::Error, 3);
                notification->show();
            }
            else{
                auto alertLayer = FLAlertLayer::create(nullptr, "DeathLink", "The server has disconnected.", "Okay", nullptr, 250);
                alertLayer->show();
            }
        }
    });
    ws.release();

#ifdef _WIN32
    WSACleanup();
#endif

}

void openLobby(){
    CCScene* scene = DeathLinkLayer::scene();
    auto transition = CCTransitionFade::create(0.5f, scene);
    CCDirector::sharedDirector()->pushScene(transition);
}

void ServerListener::onMessage(std::string message) {
    geode::Loader::get()->queueInMainThread([message]() {
        matjson::Value messageJson = matjson::parse(message.c_str());

        std::string status = messageJson["status"].as_string();

        if (messageJson.contains("lobby")) {
            std::string lobby = messageJson["lobby"].as_string();

            if(GlobalVars::getSharedInstance()->lobbyID == lobby){

                if(status == "member_list"){
                    matjson::Array members = messageJson["members"].as_array();
                    GlobalVars::getSharedInstance()->members = members;

                    CCScene* currentScene = CCDirector::sharedDirector()->getRunningScene();

                    DeathLinkLayer* deathLinkLayer = dynamic_cast<DeathLinkLayer*>(currentScene->getChildren()->objectAtIndex(0));

                    if(deathLinkLayer){
                        deathLinkLayer->reloadList();
                    }
                }

                if(status == "disconnected"){
                    GlobalVars::getSharedInstance()->lobbyID = "";

                    if(fromDeathLinkLayer()){
			            GlobalVars::getSharedInstance()->showHostError = true;
                        leaveLobbyDisconnected();
                    }
                    else{

                        PlayLayer* playLayer = PlayLayer::get();

                        if (playLayer) {
                            Notification* notification = Notification::create("The host has disconnected.", NotificationIcon::Error, 3);
                            notification->show();
                        }
                        else{
                            auto alertLayer = FLAlertLayer::create(nullptr, "DeathLink", "The host has disconnected.", "Okay", nullptr, 250);
                            alertLayer->show();
                        }
                        
                    }
                }

                if(status == "kicked"){
                    GlobalVars::getSharedInstance()->lobbyID = "";

                    if(fromDeathLinkLayer()){
			            GlobalVars::getSharedInstance()->showKickedError = true;
                        leaveLobbyDisconnected();
                    }
                    else{
                        PlayLayer* playLayer = PlayLayer::get();

                        if (playLayer) {
                            Notification* notification = Notification::create("The host has kicked you.", NotificationIcon::Error, 3);
                            notification->show();
                        }
                        else{
                            auto alertLayer = FLAlertLayer::create(nullptr, "DeathLink", "The host has kicked you.", "Okay", nullptr, 250);
                            alertLayer->show();
                        }
                    }
                }

                if(status == "rate_limit"){
                    auto alertLayer = FLAlertLayer::create(nullptr, "Slow Down!", "You have been automatically disconnected for sending too many requests to the DeathLink servers.", "Okay", nullptr, 250);
                    alertLayer->show();
                }
            }

            if(status == "created"){
                GlobalVars::getSharedInstance()->lobbyID = lobby;
                GlobalVars::getSharedInstance()->isHost = true;

                matjson::Array members = messageJson["members"].as_array();
                GlobalVars::getSharedInstance()->members = members;

                openLobby();
            }

            if(status == "message"){
                std::string globalMessage = messageJson["message"].as_string();

                auto alertLayer = FLAlertLayer::create(nullptr, "DeathLink", globalMessage, "Okay", nullptr, 250);
                alertLayer->show();
            }

            if(status == "connected"){
                GlobalVars::getSharedInstance()->lobbyID = lobby;

                matjson::Array members = messageJson["members"].as_array();
                GlobalVars::getSharedInstance()->members = members;

                openLobby();
            }

            if(status == "death"){

                PlayLayer* playLayer = PlayLayer::get();

                if (playLayer) {
                    GlobalVars::getSharedInstance()->hasDeathQueued = true;
                    playLayer->PlayLayer::destroyPlayer(playLayer->m_player1, nullptr);
		            GlobalVars::getSharedInstance()->hasDeathQueued = false;
                }
            }
        }

        if(status == "invalid"){
            auto alertLayer = FLAlertLayer::create(nullptr, "Oops!", "That lobby doesn't exist!", "Okay", nullptr, 250);
            alertLayer->show();
        }
    });
}

void ServerListener::sendMessage(std::string message) {

    if(GlobalVars::getSharedInstance()->isOpen && ws != nullptr){
        ws->send(message);
    }
}

void ServerListener::open() {

    GlobalVars::getSharedInstance()->isOpen = true;

    ws.release();
    ws.reset(WebSocket::from_url("ws://164.152.25.111:7438"));
    //ws.reset(WebSocket::from_url("ws://localhost:7438"));

}

