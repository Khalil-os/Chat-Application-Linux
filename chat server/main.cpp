#include <iostream>
#include <ChatServer.h>
#include <thread>

using namespace std;

int main()
{
    ChatServer server;

    if (server.createServerSocket() == -1)
        return 1;

    if (!server.bindServer())
        return 1;

    if (!server.listenServer())
        return 1;
    
    sockaddr_in clientAddr1{};
    int client1Socket = server.acceptClient(clientAddr1);
    if (client1Socket == -1)
        return 1;

    sockaddr_in clientAddr2{};
    int client2Socket = server.acceptClient(clientAddr2);
    if (client2Socket == -1)
        return 1;

    thread t1(&ChatServer::forwardMessage, &server, client1Socket, client2Socket);
    thread t2(&ChatServer::forwardMessage, &server, client2Socket, client1Socket);

    t1.join();
    t2.join();

    server.closeSock();
    close(client1Socket);
    close(client2Socket);
    return 0;
}