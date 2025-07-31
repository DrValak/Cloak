#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>             // close()
#include <arpa/inet.h>          // inet_addr()
#include <sys/socket.h>
#include <netinet/in.h>         // struct sockaddr_in

#define BACKLOG 5

// Iniciar servidor
int start_server(int port) {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket failed");
        return -1;
    }

    // Zero out structure
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Liga a qualquer IP
    server_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return -1;
    }

    if (listen(sockfd, BACKLOG) < 0) {
        perror("Listen failed");
        close(sockfd);
        return -1;
    }

    printf("[SERVER] A escutar na porta %d...\n", port);
    return sockfd;  // Retorna o socket para aceitar ligações
}

// Iniciar cliente
int start_client(const char *ip, int port) {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket failed");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect failed");
        close(sockfd);
        return -1;
    }

    printf("[CLIENT] Ligado a %s:%d\n", ip, port);
    return sockfd;
}

// Enviar dados
int send_data(int sockfd, uint8_t *data, size_t length) {
    ssize_t sent = send(sockfd, data, length, 0);
    if (sent < 0) {
        perror("Send failed");
        return -1;
    }
    return (int)sent;
}

// Receber dados
int receive_data(int sockfd, uint8_t *buffer, size_t buffer_size) {
    ssize_t received = recv(sockfd, buffer, buffer_size, 0);
    if (received < 0) {
        perror("Receive failed");
        return -1;
    }
    return (int)received;
}