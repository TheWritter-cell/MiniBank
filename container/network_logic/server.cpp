#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include "json.hpp"
#pragma comment(lib, "ws2_32.lib")

using json = nlohmann::json;

// Compile with : g++ server.cpp -o sr -lws2_32


class Server {
private:
    int server_port;
    SOCKET server_socket;
    std::vector<int> AdminConnection{5001,5002};
public:
    Server(int port) : server_port(port) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed.\n";
            exit(1);
        }

        server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (server_socket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed.\n";
            WSACleanup();
            exit(1);
        }

        sockaddr_in serveraddr{};
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(server_port);
        serveraddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(server_socket, (SOCKADDR*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
            std::cerr << "Bind failed.\n";
            closesocket(server_socket);
            WSACleanup();
            exit(1);
        }

        listen(server_socket, SOMAXCONN);
        std::cout << "Server started on port " << server_port << "\n";
    }

    void HandleClient(SOCKET clientSocket) {
        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived > 0) {
            std::string receivedData(buffer, bytesReceived);
            std::cout << "Received: " << receivedData << std::endl;

            try {
                json reponse = {
                    {"message", "Salut "},
                    {"status", "ok"}
                };
                std::string reponseStr = reponse.dump();
                send(clientSocket, reponseStr.c_str(), reponseStr.length(), 0);
            } catch (const json::parse_error& e) {
                std::cerr << "Erreur de parsing JSON : " << e.what() << "\n";
            }
        }

        closesocket(clientSocket);
    }

    void Stop() {
        closesocket(server_socket);
        WSACleanup();
    }

    void start_server() {
        while (true) {
            SOCKET client = accept(server_socket, nullptr, nullptr);
            if (client != INVALID_SOCKET) {
                std::thread(&Server::HandleClient, this, client).detach(); // take care of multiply clients
            }
        }
        Stop();
    }
};

int main() {
    Server trys(5000);
    trys.start_server();
    return 0;
}
