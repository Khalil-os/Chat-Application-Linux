#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <atomic>
#include "ChatUI.h"

class ChatClient
{
    private:
        int clientSocket = -1;
        std::atomic<bool> running{true};
    public:
        int createSocket(sockaddr_in &serverAddr, const std::string &ip, int port);
        bool connectToServer(sockaddr_in &serverAddr);
        void sendMessage(ChatUI &UI);
        void receiveMessage(ChatUI &UI);
        void shutdownSocket();
        void closeSock();
        void stop() { running = false; shutdownSocket(); }
        bool isRunning() const { return running.load(); }
        int getSocket() const { return clientSocket; }
};
