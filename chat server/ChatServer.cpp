#include <iostream>
#include <ChatServer.h>

using namespace std;

int ChatServer::createServerSocket()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        cout << "Socket failed!" << endl;
        return -1;
    }
    cout << "socket initialized successfully!" << endl;
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    return sock;
}

bool ChatServer::bindServer()
{
    if (bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        cout << "Bind failed !" << endl;
        close(sock);
        return false;
    }
    cout << "Bind initialized successfully!" << endl;
    return true;
}

bool ChatServer::listenServer()
{
    if (listen(sock, backlog) == -1)
    {
        cout << "Listen failed !" << endl;
        close(sock);
        return false;
    }
    cout << "Listen initialized successfully!" << endl;
    return true;
}

int ChatServer::acceptClient(sockaddr_in &clientAddr)
{
    socklen_t clientAddr1Size = sizeof(clientAddr);
    int clientSocket = accept(sock, (sockaddr*)&clientAddr, &clientAddr1Size);
    if (clientSocket == -1)
    {
        cout << "Client Socket failed!" << endl;
        close(sock);
        return -1;
    }
    cout << "Client Connected successfully!" << endl;
    return clientSocket;
}

void ChatServer::forwardMessage(int client1Socket, int client2Socket)
{
    while (true)
    {
        char buffer[1024];
        int result = recv(client1Socket, buffer, sizeof(buffer) - 1, 0);
        if (result == -1)
            return ;
        else if (result == 0)
            return;
        
        buffer[result] = '\0';
        if (send(client2Socket, buffer, result, 0) == -1)
            return ;
    }
}

int ChatServer::getSock()
{
    return sock;
}

sockaddr_in ChatServer::getServerAddr()
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