#include <iostream>
#include <string>
#include <thread>
#include "json.hpp"
#include <ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") // link with winsock


using json=nlohmann::json;

class Server{
        private:
            int sever_address;
            SOCKET server_socket;
        public:
            //constructor
            Server(int server_address){
            WSADATA wsaData;
            WSAStartup(MAKEWORD(2, 2), &wsaData); // Initialiser WinSock
            server_socket=socket(AF_INET, SOCK_STREAM , IPPROTO_TCP);
            sockaddr_in serveraddr{};
            serveraddr.sin_family=AF_INET;
            serveraddr.sin_port=htons(server_address);
            serveraddr.sin_addr.s_addr=INADDR_ANY;
            bind(server_socket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
            listen(server_socket, SOMAXCONN);
            };
            
            
            
            
            
            
            
            
            
            void Stop(){
                closesocket(server_socket);
            WSACleanup();
            }
};