#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <direct.h>
#include <io.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 3000  
#define BUFFER_SIZE 1024
#define WWW_DIR "www"


int is_valid_path(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}


void send_error(SOCKET client, int error_code) {
    const char* response;
    switch(error_code) {
        case 400:
            response = "HTTP/1.1 400 Bad Request\r\n\r\nBad Request";
            break;
        case 404:
            response = "HTTP/1.1 404 Not Found\r\n\r\nNot Found";
            break;
        default:
            response = "HTTP/1.1 500 Internal Server Error\r\n\r\nInternal Error";
    }
    send(client, response, strlen(response), 0);
}


const char* get_mime_type(const char* filename) {
    const char* ext = strrchr(filename, '.');
    if (!ext) return "application/octet-stream";
    
    if (strcmp(ext, ".html") == 0) return "text/html";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".js") == 0) return "application/javascript";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".mp4") == 0) return "video/mp4";
    if (strcmp(ext, ".pdf") == 0) return "application/pdf"; 
    
    return "application/octet-stream";
}

void send_file(SOCKET client, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        send_error(client, 404);
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* file_content = (char*)malloc(file_size);
    fread(file_content, 1, file_size, file);
    fclose(file);

    const char* mime_type = get_mime_type(filename);
    char headers[BUFFER_SIZE];
    snprintf(headers, BUFFER_SIZE,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "\r\n",
        mime_type, file_size);
    send(client, headers, strlen(headers), 0);
    send(client, file_content, file_size, 0);

    free(file_content);
}

void handle_client(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];    
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        perror("recv failed");
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

    printf("Requested method: %s\n", method);  
    printf("Requested path: %s\n", path);  

    char full_path[BUFFER_SIZE];
    if (strcmp(path, "/") == 0) {
        snprintf(full_path, sizeof(full_path), "%s/index.html", WWW_DIR);
    } else {
        snprintf(full_path, sizeof(full_path), "%s/%s", WWW_DIR, path + 1);
    }
    printf("Normalized path: %s\n", full_path); 

    // Validate and serve the file
    if (!is_valid_path(full_path)) {
        printf("Failed to open file: %s (errno: %d)\n", full_path, errno);  
        snprintf(full_path, sizeof(full_path), "%s/Error404.html", WWW_DIR);
    }

    send_file(client_socket, full_path);
    closesocket(client_socket);
}


int main() {
        
    WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", iResult);
        return 1;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        perror("Socket creation failed");
        WSACleanup();
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        int error_code = WSAGetLastError();
        fprintf(stderr, "Bind failed: %d\n", error_code);
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server is running on port %d\n", PORT);

   
    while (1) {
        SOCKET client_socket;
        struct sockaddr_in client_addr;
        int addrlen = sizeof(client_addr);

        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addrlen);
        if (client_socket == INVALID_SOCKET) {
            continue;
        }        
    }

}