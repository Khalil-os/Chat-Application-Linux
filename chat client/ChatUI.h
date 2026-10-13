#pragma once

#include <ncurses.h>
#include <vector>
#include <utility>
#include <string>

using namespace std;

class ChatUI
{
    private:
        int chatHeight = 0;
        int cols = 0;
    
    public:
        vector<pair<int, string>> messages;
        WINDOW *chatWin = nullptr;
        WINDOW *inputWin = nullptr;
        
        void initWindows();
        void displayMessages();
        void closeWin();
};