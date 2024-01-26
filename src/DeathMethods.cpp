
#include "ServerListener.h"
#include "DeathMethods.h"
#include "GlobalVars.h"
#include "DeathLinkMenu.h"
#include <algorithm>

void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

bool containsCharNotInList(std::string str){

    std::string list = "0123456789abcdefghijklmnopqrstuvwxyz ";

    for(auto elem : str){

        bool isInList = false;
        for(auto elem2 : list){

            if(elem == elem2){
                isInList = true;
            }
        }

        if(!isInList) return true;
    }
    return false;
}

void DeathMethods::link(CCObject* obj) {

    InputPrompt* p = InputPrompt::create("Link Deaths to Player", "Username", [obj](const char* val) -> void {

        std::string input = val;

        ltrim(input);
        rtrim(input);

        std::string lower;

        for(auto elem : input)
           lower += std::tolower(elem);

        std::string lowerUsername;

        for(auto elem : std::string(GJAccountManager::sharedState()->m_username))
           lowerUsername += std::tolower(elem);

        if(lower == lowerUsername){

            auto alertLayer = FLAlertLayer::create(nullptr, "Oops", "You can't link to yourself silly :P", "Okay", nullptr, 250);
            alertLayer->show();
            return;
        }

        if (containsCharNotInList(lower)){
            auto alertLayer = FLAlertLayer::create(nullptr, "Oops", "Those characters aren't allowed :(", "Okay", nullptr, 250);
            alertLayer->show();
            return;
        }

        if(input.length() > 0){
            ServerListener::sendMessage("link:" + std::string(val));
            GlobalVars::getSharedInstance()->disconnected = false;
            GlobalVars::getSharedInstance()->linkedTo = input;

            TextArea* text = dynamic_cast<TextArea*>(DeathLinkMenu::get()->m_mainLayer->getChildByTag(16283));

            text->setString(fmt::format("Linked to <cg>{}</c>", input).c_str());
            text->setContentSize({164, text->getContentSize().height});

        }
        }, "Link")->setFilter("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ");

    
    p->show();

  
}

void DeathMethods::unlink(CCObject* obj) {

    GlobalVars::getSharedInstance()->disconnected = true;
   
    if(GlobalVars::getSharedInstance()->linkedTo == ""){
        auto alertLayer = FLAlertLayer::create(nullptr, "Oops", "You aren't linked to anyone silly :P", "Okay", nullptr, 250);

        alertLayer->show();
        return;
    }

    auto alertLayer = FLAlertLayer::create(nullptr, "Success", "You have been unlinked.", "Okay", nullptr, 250);

    alertLayer->show();

    GlobalVars::getSharedInstance()->linkedTo = "";
    
    TextArea* text = dynamic_cast<TextArea*>(DeathLinkMenu::get()->m_mainLayer->getChildByTag(16283));

    text->setString("Linked to <cr>nobody</c>");
    text->setContentSize({164, text->getContentSize().height});
}

void DeathMethods::showMenu(CCObject* obj) {

    if(!GlobalVars::getSharedInstance()->isOpen){
        ServerListener::connectAsync();
    }

    DeathLinkMenu* menu = DeathLinkMenu::create();
    menu->show();
  
}
