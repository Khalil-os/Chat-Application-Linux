#include <thread>
#include <iostream>
#include <functional>
#include "ChatClient.h"

using namespace std;

int main()
{
    initscr();
    cbreak();
    noecho();

    ChatUI UI;
    UI.initWindows();

    ChatClient Client;
    
    sockaddr_in serverAddr{};

    if (Client.createSocket(serverAddr) == -1)
        return 1;
    
    if (!Client.connectToServer(serverAddr))
        return 1;

    thread Send(&ChatClient::sendMessage, &Client, ref(UI));
    thread Recv(&ChatClient::receiveMessage, &Client, ref(UI));

    Send.join();
    Recv.join();
    
    UI.closeWin();
    Client.closeSock();
    
    return 0;
}