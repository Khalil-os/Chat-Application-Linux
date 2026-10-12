#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <thread>
#include <ncurses.h>
#include <vector>
#include <functional>
#include <utility>

using namespace std;

int createSocket(sockaddr_in *serverAddr)
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1)
    {
        cout << "Socket failed!" << endl;
        return -1;
    }
    //cout << "socket initialized successfully!" << endl;
    
    
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
    //cout << "Connect initialized successfully!" << endl;
    return true;
}

void displayMessages(WINDOW* chatWin,
    vector<pair<int, string>>& messages, int chatHeight)
{
    werase(chatWin);
    box(chatWin, 0, 0);
    mvwprintw(chatWin, 1, 2, "Chat Messages");

    int maxMessages = chatHeight - 4;
    int start = 0;

    if ((int)messages.size() > maxMessages)
        start = messages.size() - maxMessages;

    int cols = getmaxx(chatWin);
    int line = 3;

    for (int j = start; j < (int)messages.size(); j++)
    {
        int clientId = messages[j].first;
        string msg = messages[j].second;

        string text = "Client " + to_string(clientId) + ": " + msg;

        int x;

        if (clientId == 2)
            x = cols - text.length() - 2;
        else
            x = 1;

        if (x < 1)
            x = 1;

        mvwprintw(chatWin, line, x, "%s", text.c_str());

        line++;
    }

    wrefresh(chatWin);
}

void sendMessage(int clientSocket, WINDOW* chatWin, WINDOW* inputWin,
    vector<pair<int, string>>& messages,int chatHeight)
{
    while (true)
    {
        char buffer[1024];

        echo();
        wmove(inputWin, 1, 11);
        wgetnstr(inputWin, buffer, 1023);
        noecho();

        messages.push_back({2, buffer});
        if (send(clientSocket, buffer, strlen(buffer), 0) == -1)
        {
            close(clientSocket);
            return;
        }
        displayMessages(chatWin, messages, chatHeight);
        werase(inputWin);
        box(inputWin, 0, 0);
        mvwprintw(inputWin, 1, 2, "Message: ");
        wrefresh(inputWin);
    }
}

void receiveMessage(int clientSocket, WINDOW* chatWin, vector<pair<int, string>>& messages, int chatHeight)
{
    while (true)
    {
        char buffer[1024];
        int result = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (result == -1)
        {
            close(clientSocket);
            return;
        }
        else if (result == 0)
            return;
        buffer[result] = '\0';
        messages.push_back({1, buffer});
        displayMessages(chatWin, messages, chatHeight);
    }
}

void initWindows(WINDOW*& chatWin, WINDOW*& inputWin, int& chatHeight, int& cols)
{
    int rows;
    getmaxyx(stdscr, rows, cols);

    int inputHeight = 3;
    chatHeight = rows - inputHeight;

    chatWin = newwin(chatHeight, cols, 0, 0);
    inputWin = newwin(inputHeight, cols, chatHeight, 0);

    box(chatWin, 0, 0);
    box(inputWin, 0, 0);

    mvwprintw(chatWin, 1, 2, "Chat Messages");
    mvwprintw(inputWin, 1, 2, "Message: ");

    wrefresh(chatWin);
    wrefresh(inputWin);
}

int main()
{
    initscr();
    cbreak();
    noecho();

    WINDOW *chatWin;
    WINDOW *inputWin;

    vector<pair<int, string>> messages;

    int chatHeight, cols;

    initWindows(chatWin, inputWin, chatHeight, cols);

    sockaddr_in serverAddr{};
    int clientSocket = createSocket(&serverAddr);
    if (clientSocket == -1)
        return 1;
    
    if (!connectToServer(clientSocket, &serverAddr))
        return 1;

    thread Send(sendMessage, clientSocket, chatWin, inputWin, ref(messages), chatHeight);
    thread Recv(receiveMessage, clientSocket, chatWin, ref(messages), chatHeight);

    Send.join();

    Recv.join();
    
    delwin(chatWin);
    delwin(inputWin);
    endwin();
    close(clientSocket);
    return 0;
}