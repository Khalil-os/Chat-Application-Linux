#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

using namespace std;

int createSocket(sockaddr_in *serverAddr)
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1)
    {
        cout << "Socket failed!" << endl;
        return -1;
    }
    cout << "socket initialized successfully!" << endl;
    
    
    serverAddr->sin_family = AF_INET;
    serverAddr->sin_port = htons(5000);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr->sin_addr) != 1)
    {
        cout << "Inet Pton failed!" << endl;
        close(clientSocket);
        return -1;
    }

    return clientSocket;
}

bool connectToServer(int clientSocket, sockaddr_in *serverAddr)
{
    if (connect(clientSocket, (sockaddr*)serverAddr, sizeof(*serverAddr)) != 0)
    {
        cout << "Connect failed!" << endl;
        close(clientSocket);
        return false;
    }
    cout << "Connect initialized successfully!" << endl;
    return true;
}

bool sendMessage(int clientSocket)
{
    string msg;
    cout << endl << "Enter msg to send to client 2 : ";
    getline(cin, msg);
    if (send(clientSocket, msg.c_str(), msg.size(), 0) == -1)
    {
        cout << "Send failed!" << endl;
        close(clientSocket);
        return false;
    }
    cout << endl;
    return true;
}

int receiveMessage(int clientSocket)
{
    char buffer[1024];
    int result = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (result == -1)
    {
        cout << "send msg failed!" << endl;
        close(clientSocket);
        return -1;
    }
    else if (result == 0)
        return 0;
    buffer[result] = '\0';
    cout << "Client 2 : " << buffer << endl;
    return 1;
}

int main()
{
    sockaddr_in serverAddr{};
    int clientSocket = createSocket(&serverAddr);
    if (clientSocket == -1)
        return 1;
    
    if (!connectToServer(clientSocket, &serverAddr))
        return 1;

    while (true)
    {
        if (!sendMessage(clientSocket))
            return 1;

        int msg = receiveMessage(clientSocket);
        if (msg == -1)
            return 1;
        else if(msg == 0)
            break;
    }
    
    close(clientSocket);
    return 0;
}