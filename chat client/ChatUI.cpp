#include "ChatUI.h"
#include <mutex>

void ChatUI::displayMessages()
{
    std::lock_guard<std::mutex> lock(mtx);

    werase(chatWin);
    box(chatWin, 0, 0);
    mvwprintw(chatWin, 1, 2, "Chat Messages");

    int maxMessages = chatHeight - 4;
    int start = 0;
    if ((int)messages.size() > maxMessages)
        start = (int)messages.size() - maxMessages;

    int winCols = getmaxx(chatWin);
    int usable = winCols - 4;
    if (usable < 1) usable = 1;

    int line = 3;
    for (int j = start; j < (int)messages.size(); j++)
    {
        int clientId = messages[j].first;
        std::string msg = messages[j].second;
        std::string prefix = (clientId == 1) ? "You: " : "Other: ";
        std::string text = prefix + msg;

        // wrap long messages
        int offset = 0;
        bool firstLine = true;
        while (offset < (int)text.length() && line < chatHeight - 1)
        {
            std::string chunk = text.substr(offset, usable);
            int x;
            if (clientId == 1 && firstLine)
            {
                x = winCols - (int)chunk.length() - 2;
                if (x < 1) x = 1;
            }
            else
            {
                x = 2;
            }
            mvwprintw(chatWin, line, x, "%s", chunk.c_str());
            offset += usable;
            firstLine = false;
            if (offset < (int)text.length())
                line++;
        }
        line++;
        if (line >= chatHeight - 1) break;
    }
    wrefresh(chatWin);
}

void ChatUI::addMessage(int id, const std::string &msg)
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        messages.emplace_back(id, msg);
    }
    displayMessages();
}

void ChatUI::initWindows()
{
    int rows;
    getmaxyx(stdscr, rows, cols);

    int inputHeight = 3;
    chatHeight = rows - inputHeight;

    chatWin = newwin(chatHeight, cols, 0, 0);
    inputWin = newwin(inputHeight, cols, chatHeight, 0);

    box(chatWin, 0, 0);
    box(inputWin, 0, 0);

    mvwprintw(chatWin, 1, 2, "Chat Messages");
    mvwprintw(inputWin, 1, 2, "Message: ");

    wrefresh(chatWin);
    wrefresh(inputWin);
}

void ChatUI::closeWin()
{
    if (chatWin)
    {
        delwin(chatWin);
        chatWin = nullptr;
    }
    if (inputWin)
    {
        delwin(inputWin);
        inputWin = nullptr;
    }
    endwin();
}
