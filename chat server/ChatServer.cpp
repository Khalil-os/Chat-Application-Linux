#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define backlog 5

using namespace std;

int Send_msg(int client1Socket, int client2Socket)
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
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        cout << "Socket failed!" << endl;
        return 1;
    }
    cout << "socket initialized successfully!" << endl;
    
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    
    if (bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        cout << "Bind failed !" << endl;
        close(sock);
        return 1;
    }
    cout << "Bind initialized successfully!" << endl;

    if (listen(sock, backlog) == -1)
    {
        cout << "Listen failed !" << endl;
        close(sock);
        return 1;
    }
    cout << "Listen initialized successfully!" << endl;

    sockaddr_in clientAddr1{};

    socklen_t clientAddr1Size = sizeof(clientAddr1);
    int client1Socket = accept(sock, (sockaddr*)&clientAddr1, &clientAddr1Size);
    if (client1Socket == -1)
    {
        cout << "Client 1 Socket failed!" << endl;
        close(sock);
        return 1;
    }
    cout << "Client 1 Connected successfully!" << endl;

    sockaddr_in clientAddr2{};

    socklen_t clientAddr2Size = sizeof(clientAddr2);
    int client2Socket = accept(sock, (sockaddr*)&clientAddr2, &clientAddr2Size);
    if (client2Socket == -1)
    {
        cout << "Client 2 Socket failed!" << endl;
        close(sock);
        close(client1Socket);
        return 1;
    }
    cout << "Client 2 Connected successfully!" << endl;

    while (true)
    {
        int result1 = Send_msg(client1Socket, client2Socket);
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

        int result2 = Send_msg(client2Socket, client1Socket);
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