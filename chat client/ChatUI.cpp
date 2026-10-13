#include "ChatUI.h"

void ChatUI::displayMessages()
{
    werase(chatWin);
    box(chatWin, 0, 0);
    mvwprintw(chatWin, 1, 2, "Chat Messages");

    int maxMessages = chatHeight - 4;
    int start = 0;

    if ((int)messages.size() > maxMessages)
        start = messages.size() - maxMessages;

    int cols = getmaxx(chatWin);
    int line = 3;

    for (int j = start; j < (int)messages.size(); j++)
    {
        int clientId = messages[j].first;
        string msg = messages[j].second;

        string text = "Client " + to_string(clientId) + ": " + msg;

        int x;

        if (clientId == 1)
            x = cols - text.length() - 2;
        else
            x = 2;

        if (x < 1)
            x = 1;

        mvwprintw(chatWin, line, x, "%s", text.c_str());

        line++;
    }

    wrefresh(chatWin);
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
    delwin(chatWin);
    chatWin = nullptr;
    delwin(inputWin);
    inputWin = nullptr;
    endwin();
}