#pragma once
#include <algorithm>

#ifndef __UTILS_H
#define __UTILS_H

namespace Utils {

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
}

#endif