#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define backlog 5

class ChatServer
{
    private:
        int sock = -1;
        sockaddr_in serverAddr{};
    public:
        int createServerSocket();
        bool bindServer();
        bool listenServer();
        int acceptClient(sockaddr_in &clientAddr);
        void closeSock();
        void forwardMessage(int client1Socket, int client2Socket);
        
        int getSock();
        sockaddr_in getServerAddr();

};