#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

class ChatServer
{
    private:
        int sock = -1;
        sockaddr_in serverAddr{};
        static constexpr int BACKLOG = 5;
        static constexpr int DEFAULT_PORT = 5000;

    public:
        int createServerSocket(int port = DEFAULT_PORT);
        bool bindServer();
        bool listenServer();
        int acceptClient(sockaddr_in &clientAddr);
        void closeSock();
        void forwardMessage(int client1Socket, int client2Socket);

        int getSock() const;
        sockaddr_in getServerAddr() const;
};
