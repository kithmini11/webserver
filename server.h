#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>

void handle_client(SOCKET client_socket);
void initialize_server();
void send_error(SOCKET client_socket, int status_code);
void send_response(SOCKET client_socket, const char *file_path);

#endif