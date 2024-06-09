#include <cstdio>
#include <WinSock2.h>
#include <cstring>
#include <winsock.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdlib.h>
#include <mingw.thread.h>
#include <iostream>
#include <string>

// max length of the buffer holding message
#define MAX_LEN 0xff

// #pragma comment(lib, "ws2_32.lib")

// check if any mistake happens, if so print it to console and shut down the program
void error_check(int ret, std::string alert);

int main()
{
    // ret holds the return value when we call APIs
    int ret;

    // initialize network environment
    WSADATA wsadata;
    ret = WSAStartup(MAKEWORD(2, 2), &wsadata);

    // create the socket of the server
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    error_check((int)server_socket, "fail to create socket!");

    // bind the socket to the address of this computer
    sockaddr_in address;
    address.sin_family = AF_INET;                                   // IPv4
    address.sin_port = htons(9999);                      // port 9999
    address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");      // IP address

    ret = bind(server_socket, (sockaddr*)&address, sizeof(address));
    error_check(ret, "fail to bind the port as well as address!");

    // make the socket listen to connection request
    ret = listen(server_socket, SOMAXCONN);
    error_check(ret, "fail to listen connection request!");

    // create the socket for two clients
    sockaddr client1_address;
    sockaddr client2_address;

    int length1 = sizeof(client1_address);
    int length2 = sizeof(client2_address);
    
    SOCKET client1_socket = 0;
    SOCKET client2_socket = 0;

    // wait for client1 to connect our server
    std::cout << "waiting for client1 to join..." << std::endl;
    client1_socket = accept(server_socket, &client1_address, &length1);
    error_check((int)client1_socket, "fail to connect to client1!");

    // print log
    std::cout << "successfully connect to client1: " << (int)client1_socket << std::endl;

    // wait for client2 to connect our server
    std::cout << "waiting for client2 to join..." << std::endl;
    client2_socket = accept(server_socket, &client2_address, &length2);
    error_check((int)client2_socket, "fail to connect to client2!"); 

    // print log
    std::cout << "successfully connect to client2: " << (int)client2_socket << std::endl;

    char buffer1[MAX_LEN];
    char buffer2[MAX_LEN];

    // notice both client to start a new game
    sprintf(buffer1, "start");
    sprintf(buffer2, "start");

    ret = send(client1_socket, buffer1, strlen(buffer1), 0);
    error_check(ret, "fail to send start message to client1!");
    
    ret = send(client2_socket, buffer2, strlen(buffer2), 0);
    error_check(ret, "fail to send start message to client2!");

    // keep exchanging score information until both game are over
    while (strcmp(buffer1, "over") != 0 || strcmp(buffer2, "over") != 0)
    {
        // receive message from two clients
        ret = recv(client1_socket, buffer1, strlen(buffer1), 0);
        error_check(ret, "fail to receive message from client1!");
        
        ret = recv(client2_socket, buffer2, strlen(buffer2), 0);
        error_check(ret, "fail to receive message from client2!");

        // send score information to each other
        ret = send(client1_socket, buffer2, strlen(buffer2), 0);
        error_check(ret, "fail to send score message to client1!");
        
        ret = send(client2_socket, buffer1, strlen(buffer2), 0);
        error_check(ret, "fail to send score message to client2!");
    }

    // inform both client that the connection is going to close
    sprintf(buffer1, "close");
    sprintf(buffer2, "close");

    ret = send(client1_socket, buffer1, strlen(buffer1), 0);
    error_check(ret, "fail to send close message to client1!");
    
    ret = send(client2_socket, buffer2, strlen(buffer2), 0);
    error_check(ret, "fail to send close message to client2!");

    // close all socket
    closesocket(server_socket);
    closesocket(client1_socket);
    closesocket(client2_socket);

    // release the network environment
    WSACleanup();
    return 0;
}

void error_check(int ret, std::string alert)
{
    // print details to console when we meet mistakes
    if (ret == -1)
    {
        std::cout << alert << std::endl;
        
        // release the network environment
        WSACleanup();
        exit(-1);
    }
}