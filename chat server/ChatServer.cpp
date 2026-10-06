#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define backlog 5

using namespace std;

int createServerSocket(sockaddr_in *serverAddr)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        cout << "Socket failed!" << endl;
        return -1;
    }
    cout << "socket initialized successfully!" << endl;
    
    serverAddr->sin_family = AF_INET;
    serverAddr->sin_port = htons(5000);
    serverAddr->sin_addr.s_addr = INADDR_ANY;
    return sock;
}

bool bindServer(int sock, sockaddr_in *serverAddr)
{
    if (bind(sock, (sockaddr*)serverAddr, sizeof(*serverAddr)) == -1)
    {
        cout << "Bind failed !" << endl;
        close(sock);
        return false;
    }
    cout << "Bind initialized successfully!" << endl;
    return true;
}

bool listenServer(int sock)
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

int acceptClient(int sock, sockaddr_in *clientAddr)
{
    socklen_t clientAddr1Size = sizeof(*clientAddr);
    int clientSocket = accept(sock, (sockaddr*)clientAddr, &clientAddr1Size);
    if (clientSocket == -1)
    {
        cout << "Client Socket failed!" << endl;
        close(sock);
        return -1;
    }
    cout << "Client Connected successfully!" << endl;
    return clientSocket;
}

int forwardMessage(int client1Socket, int client2Socket)
{
    char buffer[1024];
    int result = recv(client1Socket, buffer, sizeof(buffer) - 1, 0);
    if (result == -1)
        return -1;
    else if (result == 0)
        return 0;
    
    buffer[result] = '\0';
    if (send(client2Socket, buffer, result, 0) == -1)
        return -1;
    return 1;
}

int main()
{
    sockaddr_in serverAddr{};
    int sock = createServerSocket(&serverAddr);
    if (sock == -1)
        return 1;

    if (!bindServer(sock, &serverAddr))
        return 1;

    if (!listenServer(sock))
        return 1;
    
    sockaddr_in clientAddr1{};
    int client1Socket = acceptClient(sock, &clientAddr1);
    if (client1Socket == -1)
        return 1;

    sockaddr_in clientAddr2{};
    int client2Socket = acceptClient(sock, &clientAddr2);
    if (client2Socket == -1)
        return 1;

    while (true)
    {
        int result1 = forwardMessage(client1Socket, client2Socket);
        if (result1 == -1)
        {
            cout << "send msg failed!" << endl;
            close(sock);
            close(client1Socket);
            close(client2Socket);
            return 1;
        }
        else if (result1 == 0)
            break;

        int result2 = forwardMessage(client2Socket, client1Socket);
        if (result2 == -1)
        {
            cout << "send msg failed!" << endl;
            close(sock);
            close(client1Socket);
            close(client2Socket);
            return 1;
        }
        else if (result2 == 0)
            break;
    }

    close(sock);
    close(client1Socket);
    close(client2Socket);
    return 0;
}