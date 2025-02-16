#ifndef MUTILS_H
#define MUTILS_H

#include <winsock2.h>
#include <stdbool.h>
#include <string.h>

bool is_valid_path(const char *path);
void send_error(SOCKET client_socket, int error_code);
void send_response(SOCKET client_socket, const char* file_path);

#endif