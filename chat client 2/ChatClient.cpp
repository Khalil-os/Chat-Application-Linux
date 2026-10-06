#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

using namespace std;

int main()
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1)
    {
        cout << "Socket failed!" << endl;
        return 1;
    }
    cout << "socket initialized successfully!" << endl;
    
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) != 1)
    {
        cout << "Inet Pton failed!" << endl;
        close(clientSocket);
        return 1;
    }

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
    {
        cout << "Connect failed!" << endl;
        close(clientSocket);
        return 1;
    }
    cout << "Connect initialized successfully!" << endl;

    while (true)
    {
        string msg;
        cout << endl << "Enter msg to send to client 1 : ";
        getline(cin, msg);
        if (send(clientSocket, msg.c_str(), msg.size(), 0) == -1)
        {
            cout << "Send failed!" << endl;
            close(clientSocket);
            return 1;
        }
        cout << endl;

        char buffer[1024];
        int result = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (result == -1)
        {
            cout << "send msg failed!" << endl;
            close(clientSocket);
            return 1;
        }
        else if (result == 0)
            break;
        buffer[result] = '\0';
        cout << "Client 1 : " << buffer << endl;
    }
    
    close(clientSocket);
    return 0;
}