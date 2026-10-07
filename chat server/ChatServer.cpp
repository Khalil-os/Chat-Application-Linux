#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

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

void forwardMessage(int client1Socket, int client2Socket)
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

    thread t1(forwardMessage, client1Socket, client2Socket);
    thread t2(forwardMessage, client2Socket, client1Socket);

    t1.join();
    t2.join();

    close(sock);
    close(client1Socket);
    close(client2Socket);
    return 0;
}