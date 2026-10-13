#include <iostream>
#include <string.h>
#include "ChatClient.h"

int ChatClient::createSocket(sockaddr_in &serverAddr)
{
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1)
    {
        cout << "Socket failed!" << endl;
        return -1;
    }    
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) != 1)
    {
        cout << "Inet Pton failed!" << endl;
        close(clientSocket);
        return -1;
    }

    return clientSocket;
}

bool ChatClient::connectToServer(sockaddr_in &serverAddr)
{
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
    {
        cout << "Connect failed!" << endl;
        close(clientSocket);
        return false;
    }
    return true;
}

void ChatClient::sendMessage(ChatUI &UI)
{
    while (true)
    {
        char buffer[1024];

        echo();
        wmove(UI.inputWin, 1, 11);
        wgetnstr(UI.inputWin, buffer, 1023);
        noecho();

        UI.messages.push_back({1, buffer});
        if (send(clientSocket, buffer, strlen(buffer), 0) == -1)
        {
            close(clientSocket);
            return;
        }
        UI.displayMessages();
        werase(UI.inputWin);
        box(UI.inputWin, 0, 0);
        mvwprintw(UI.inputWin, 1, 2, "Message: ");
        wrefresh(UI.inputWin);
    }
}

void ChatClient::receiveMessage(ChatUI &UI)
{
    while (true)
    {
        char buffer[1024];
        int result = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (result == -1)
        {
            close(clientSocket);
            return;
        }
        else if (result == 0)
            return;
        buffer[result] = '\0';
        UI.messages.push_back({2, buffer});
        UI.displayMessages();
    }
}

void ChatClient::closeSock()
{
    if (clientSocket != -1)
    {
        close(clientSocket);
        clientSocket = -1;
    }
}