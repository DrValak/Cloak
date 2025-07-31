#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>
#include <stddef.h>

int start_server(int port);
int start_client(const char *ip, int port);

int send_data(int sockfd, uint8_t *data, size_t length);
int receive_data(int sockfd, uint8_t *buffer, size_t buffer_size);

#endif