#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm> // std::remove ke liye

using namespace std;

#pragma comment(lib, "ws2_32.lib")

vector<SOCKET> clients;      // All connected clients
mutex clientsMutex;          // To safely access vector

void broadcastMessage(const string &msg, SOCKET sender) {
    lock_guard<mutex> lock(clientsMutex);
    for (SOCKET c : clients) {
        if (c != sender) { // Sender ko exclude karna optional
            send(c, msg.c_str(), msg.size(), 0);
        }
    }
}

void handleClient(SOCKET clientSocket, int clientId) {
    char buffer[1024];
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            cout << "Client " << clientId << " disconnected." << endl;
            closesocket(clientSocket);

            lock_guard<mutex> lock(clientsMutex);
            // Windows ke remove() se conflict avoid karne ke liye std::remove
            clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
            break;
        }
        buffer[bytesReceived] = '\0';
        string message = "Client " + to_string(clientId) + " says: " + buffer;
        cout << message << endl;

        broadcastMessage(message, clientSocket); // Forward message to all other clients
    }
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    cout << "Server listening on port 8080..." << endl;

    int clientId = 0;
    while (true) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) continue;

        {
            lock_guard<mutex> lock(clientsMutex);
            clients.push_back(clientSocket);
        }

        cout << "New client connected: " << clientId << endl;

        thread t(handleClient, clientSocket, clientId++);
        t.detach();
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
