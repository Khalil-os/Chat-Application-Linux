#include "ChatClient.h"
#include <iostream>
#include <cstring>
#include <string>
#include <mutex>

int ChatClient::createSocket(sockaddr_in &serverAddr, const std::string &ip, int port)
{
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        std::cout << "Socket failed!" << std::endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) != 1)
    {
        std::cout << "inet_pton failed for ip: " << ip << std::endl;
        close(clientSocket);
        clientSocket = -1;
        return -1;
    }
    return clientSocket;
}

bool ChatClient::connectToServer(sockaddr_in &serverAddr)
{
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
    {
        std::cout << "Connect failed!" << std::endl;
        close(clientSocket);
        clientSocket = -1;
        return false;
    }
    return true;
}

static void redrawInput(ChatUI &UI, const std::string &input)
{
    std::lock_guard<std::mutex> lk(UI.getMutex());
    werase(UI.inputWin);
    box(UI.inputWin, 0, 0);
    mvwprintw(UI.inputWin, 1, 2, "Message: %s", input.c_str());
    wmove(UI.inputWin, 1, (int)input.length() + 11);
    wrefresh(UI.inputWin);
}

void ChatClient::sendMessage(ChatUI &UI)
{
    wtimeout(UI.inputWin, 100);
    keypad(UI.inputWin, TRUE);

    std::string input;

    redrawInput(UI, input);

    while (running)
    {
        int ch = wgetch(UI.inputWin);
        if (!running) break;

        if (ch == ERR)
            continue;

        if (ch == '\n' || ch == KEY_ENTER || ch == 10 || ch == 13)
        {
            if (input.empty())
            {
                redrawInput(UI, input);
                continue;
            }
            if (input == "/quit" || input == "/exit")
            {
                stop();
                break;
            }

            std::string toSend = input;
            input.clear();
            redrawInput(UI, input);

            UI.addMessage(1, toSend);

            if (send(clientSocket, toSend.c_str(), toSend.length(), 0) == -1)
            {
                stop();
                break;
            }
        }
        else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8)
        {
            if (!input.empty())
            {
                input.pop_back();
                redrawInput(UI, input);
            }
        }
        else if (ch >= 32 && ch < 127)
        {
            if ((int)input.length() < 900)
            {
                input.push_back((char)ch);
                redrawInput(UI, input);
            }
        }
    }
}

void ChatClient::receiveMessage(ChatUI &UI)
{
    while (running)
    {
        char buffer[1024];
        int result = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (result <= 0)
        {
            if (running)
            {
                if (result == 0)
                    UI.addMessage(2, "[Other disconnected]");
                else
                    UI.addMessage(2, "[Connection lost]");
            }
            stop();
            break;
        }
        buffer[result] = '\0';
        UI.addMessage(2, buffer);
    }
}

void ChatClient::shutdownSocket()
{
    if (clientSocket != -1)
        shutdown(clientSocket, SHUT_RDWR);
}

void ChatClient::closeSock()
{
    if (clientSocket != -1)
    {
        close(clientSocket);
        clientSocket = -1;
    }
}
