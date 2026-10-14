#pragma once

#include <ncurses.h>
#include <vector>
#include <string>
#include <utility>
#include <mutex>

class ChatUI
{
    private:
        int chatHeight = 0;
        int cols = 0;
        std::mutex mtx;

    public:
        std::vector<std::pair<int, std::string>> messages;
        WINDOW *chatWin = nullptr;
        WINDOW *inputWin = nullptr;

        void initWindows();
        void displayMessages();
        void addMessage(int id, const std::string &msg);
        void closeWin();

        std::mutex &getMutex() { return mtx; }
};
