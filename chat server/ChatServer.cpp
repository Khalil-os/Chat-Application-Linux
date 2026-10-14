#include "ChatServer.h"
#include <iostream>

int ChatServer::createServerSocket(int port)
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        std::cout << "Socket failed!" << std::endl;
        return -1;
    }

    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        std::cout << "setsockopt(SO_REUSEADDR) failed!" << std::endl;
        close(sock);
        sock = -1;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    std::cout << "Socket initialized successfully!" << std::endl;
    return sock;
}

bool ChatServer::bindServer()
{
    if (bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cout << "Bind failed!" << std::endl;
        close(sock);
        sock = -1;
        return false;
    }
    std::cout << "Bind initialized successfully!" << std::endl;
    return true;
}

bool ChatServer::listenServer()
{
    if (listen(sock, BACKLOG) == -1)
    {
        std::cout << "Listen failed!" << std::endl;
        close(sock);
        sock = -1;
        return false;
    }
    std::cout << "Listen initialized successfully!" << std::endl;
    return true;
}

int ChatServer::acceptClient(sockaddr_in &clientAddr)
{
    socklen_t clientAddrSize = sizeof(clientAddr);
    int clientSocket = accept(sock, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == -1)
    {
        std::cout << "accept() failed!" << std::endl;
        return -1;
    }
    std::cout << "Client connected successfully! (fd=" << clientSocket << ")" << std::endl;
    return clientSocket;
}

void ChatServer::forwardMessage(int client1Socket, int client2Socket)
{
    while (true)
    {
        char buffer[1024];
        int result = recv(client1Socket, buffer, sizeof(buffer) - 1, 0);
        if (result <= 0)
        {
            shutdown(client2Socket, SHUT_RDWR);
            return;
        }
        buffer[result] = '\0';
        if (send(client2Socket, buffer, result, 0) == -1)
        {
            shutdown(client1Socket, SHUT_RDWR);
            return;
        }
    }
}

int ChatServer::getSock() const
{
    return sock;
}

sockaddr_in ChatServer::getServerAddr() const
{
    return serverAddr;
}

void ChatServer::closeSock()
{
    if (sock != -1)
    {
        close(sock);
        sock = -1;
    }
}
