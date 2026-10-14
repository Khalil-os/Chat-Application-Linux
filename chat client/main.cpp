#include <thread>
#include <iostream>
#include <string>
#include <csignal>
#include "ChatClient.h"

static ChatClient *g_client = nullptr;

void handleSigint(int)
{
    if (g_client) g_client->stop();
}

int main(int argc, char *argv[])
{
    std::string ip = "127.0.0.1";
    int port = 5000;

    if (argc >= 2) ip = argv[1];
    if (argc >= 3)
    {
        try { port = std::stoi(argv[2]); }
        catch (...) { std::cerr << "Invalid port, using 5000\n"; port = 5000; }
    }

    ChatClient client;
    g_client = &client;
    std::signal(SIGINT, handleSigint);
    std::signal(SIGTERM, handleSigint);

    sockaddr_in serverAddr{};
    if (client.createSocket(serverAddr, ip, port) == -1)
        return 1;

    std::cout << "Connecting to " << ip << ":" << port << "..." << std::endl;
    if (!client.connectToServer(serverAddr))
        return 1;
    std::cout << "Connected!" << std::endl;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    ChatUI UI;
    UI.initWindows();

    std::thread tSend(&ChatClient::sendMessage, &client, std::ref(UI));
    std::thread tRecv(&ChatClient::receiveMessage, &client, std::ref(UI));

    tSend.join();
    client.stop();
    tRecv.join();

    UI.closeWin();
    client.closeSock();
    g_client = nullptr;

    std::cout << "Disconnected. Bye!" << std::endl;
    return 0;
}
