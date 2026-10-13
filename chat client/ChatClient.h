#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ChatUI.h"

class ChatClient
{
    private:
        int clientSocket = -1;
    public:
        int createSocket(sockaddr_in &serverAddr);
        bool connectToServer(sockaddr_in &serverAddr);
        void sendMessage(ChatUI &UI);
        void receiveMessage(ChatUI &UI);
        void closeSock();
};