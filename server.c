#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <direct.h>
#include "server.h"
#include "mime_types.h"
#include "mutils.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        printf("recv failed\n");
        closesocket(client_socket);
        return;
    }
    
    buffer[bytes_received] = '\0';
    char *method = strtok(buffer, " ");
    char *path = strtok(NULL, " ");
    
    if (method == NULL || path == NULL) {
        send_error(client_socket, 400);
        closesocket(client_socket);
        return;
    }

    // Handle root path
    char full_path[256];
    char cwd[256];
    _getcwd(cwd, sizeof(cwd));
    
    if (strlen(path + 1) == 0 || strcmp(path, "/") == 0) {
        snprintf(full_path, sizeof(full_path), "%s/www/index.html", cwd);
    } else {
        snprintf(full_path, sizeof(full_path), "%s/www/%s", cwd, path + 1);
    }

    printf("Attempting to serve: %s\n", full_path);  

    if (!is_valid_path(full_path)) {
        printf("File not found: %s\n", full_path); 
        send_error(client_socket, 404);
        closesocket(client_socket);
        return;
    }

    send_response(client_socket, full_path);
    closesocket(client_socket);
}

int main() {
    WSADATA wsaData;
    int iResult;

    
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Bind failed\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, 5) < 0) {
        printf("Listen failed\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server is running on port %d\n", PORT);

    while (1) {
        SOCKET client_socket;
        struct sockaddr_in client_addr;
        int addrlen = sizeof(client_addr);

        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addrlen);
        if (client_socket == INVALID_SOCKET) {
            printf("Accept failed\n");
            continue;
        }
        handle_client(client_socket);
        closesocket(client_socket);
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}