#include "mutils.h"
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <stdlib.h>

#define MIME_PDF "application/pdf"

#define BUFFER_SIZE 1024

char* normalize_path(const char* base_path, const char* relative_path) {
    static char full_path[BUFFER_SIZE];
    snprintf(full_path, sizeof(full_path), "%s%s", base_path, relative_path);
    return full_path;
}

bool is_valid_path(const char* path) {
    FILE* file = fopen(path, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

void send_error(SOCKET client_socket, int error_code) {
    const char *error_message;
    switch (error_code) {
        case 400:
            error_message = "HTTP/1.1 400 Bad Request\r\n\r\n";
            break;
        case 404:
            error_message = "HTTP/1.1 404 Not Found\r\n\r\n";
            break;
        case 500:
            error_message = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
            break;

        default:
            error_message = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
            break;
    }
    send(client_socket, error_message, strlen(error_message), 0);
}

void send_response(SOCKET client_socket, const char* file_path) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        send_error(client_socket, 404);
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    const char* content_type = "application/octet-stream";
    const char* content_disposition = "";

    if (strstr(file_path, ".html")) {
        content_type = "text/html";
    } else if (strstr(file_path, ".pdf")) {
        content_type = "application/pdf";
        content_disposition = "Content-Disposition: attachment; filename=\"download.pdf\"\r\n";
    } else if (strstr(file_path, ".css")) {
        content_type = "text/css";
    } else if (strstr(file_path, ".js")) {
        content_type = "application/javascript";
    }

    char header[BUFFER_SIZE];
    snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "%s"
        "\r\n",
        content_type,
        file_size,
        content_disposition
    );
    send(client_socket, header, strlen(header), 0);

    char buffer[8192];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }

    fclose(file);
}
