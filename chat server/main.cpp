#include "ChatServer.h"
#include <iostream>
#include <thread>
#include <csignal>
#include <string>

static ChatServer *g_server = nullptr;

void handleSigint(int)
{
    std::cout << "\nShutting down..." << std::endl;
    if (g_server) g_server->closeSock();
    std::_Exit(0);
}

int main(int argc, char *argv[])
{
    int port = 5000;
    if (argc >= 2)
    {
        try { port = std::stoi(argv[1]); }
        catch (...) { std::cerr << "Invalid port, using 5000\n"; port = 5000; }
    }

    ChatServer server;
    g_server = &server;
    std::signal(SIGINT, handleSigint);
    std::signal(SIGTERM, handleSigint);

    if (server.createServerSocket(port) == -1)
        return 1;
    if (!server.bindServer())
        return 1;
    if (!server.listenServer())
        return 1;

    std::cout << "Server listening on port " << port << " — waiting for 2 clients..." << std::endl;

    sockaddr_in clientAddr1{};
    int client1Socket = server.acceptClient(clientAddr1);
    if (client1Socket == -1)
        return 1;

    std::cout << "Client 1 connected, waiting for Client 2..." << std::endl;

    sockaddr_in clientAddr2{};
    int client2Socket = server.acceptClient(clientAddr2);
    if (client2Socket == -1)
    {
        close(client1Socket);
        return 1;
    }

    std::cout << "Both clients connected — relaying messages." << std::endl;

    std::thread t1(&ChatServer::forwardMessage, &server, client1Socket, client2Socket);
    std::thread t2(&ChatServer::forwardMessage, &server, client2Socket, client1Socket);

    t1.join();
    t2.join();

    std::cout << "A client disconnected — shutting down." << std::endl;

    close(client1Socket);
    close(client2Socket);
    server.closeSock();
    g_server = nullptr;
    return 0;
}
