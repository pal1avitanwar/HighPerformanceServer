#include <iostream>
#include <winsock2.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Connection failed!" << endl;
        return 1;
    }

    cout << "Connected to server!" << endl;

    while (true) {
        string msg;
        cout << "You: ";
        getline(cin, msg);

        send(sock, msg.c_str(), msg.size(), 0);

        char buffer[1024];
        int bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            cout << "Server echoed: " << buffer << endl;
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
