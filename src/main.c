#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include "ring_buffer.h"
#include "crypt.h"
#include "network.h"

#define ENCRYPTION_KEY 0x3F

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage:\n");
        printf("  As server: %s server <port>\n", argv[0]);
        printf("  As client: %s client <ip> <port>\n", argv[0]);
        return 1;
    }

    int sockfd;
    if (strcmp(argv[1], "server") == 0) {
        int port = atoi(argv[2]);
        sockfd = start_server(port);
        printf("[Server] Listening on port %d...\n", port);
    } else if (strcmp(argv[1], "client") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Client mode requires IP and port\n");
            return 1;
        }
        const char *ip = argv[2];
        int port = atoi(argv[3]);
        sockfd = start_client(ip, port);
        printf("[Client] Connected to %s:%d\n", ip, port);
    } else {
        fprintf(stderr, "Invalid mode: use 'server' or 'client'\n");
        return 1;
    }

    CircularBuffer buffer;
    startBuffer(&buffer);
    char input[BUFFER_SIZE];

    while (1) {
        printf("Enter message: ");
        if (!fgets(input, sizeof(input), stdin)) break;

        size_t len = strlen(input);
        if (input[len - 1] == '\n') input[len - 1] = '\0';
        len = strlen(input);  // atualizar comprimento real

        // Adiciona ao buffer
        for (size_t i = 0; i < len; i++) {
            if (addToBuffer(&buffer, (uint8_t)input[i]) != 0) {
                fprintf(stderr, "Buffer full. Message truncated.\n");
                break;
            }
        }

        // Obter mensagem do buffer e encriptar
        uint8_t message[BUFFER_SIZE];
        size_t count = (buffer.full) ? BUFFER_SIZE :
                      (buffer.tail >= buffer.head) ? buffer.tail - buffer.head :
                      BUFFER_SIZE - buffer.head + buffer.tail;

        for (size_t i = 0; i < count; i++) {
            removeToBuffer(&buffer, &message[i]);
        }

        xor_encrypt(message, count, ENCRYPTION_KEY);
        send_data(sockfd, message, count);

        // Receber resposta
        uint8_t response[BUFFER_SIZE];
        int received = receive_data(sockfd, response, BUFFER_SIZE);
        if (received > 0) {
            xor_decrypt(response, received, ENCRYPTION_KEY);
            printf("Received: %.*s\n", received, response);
        } else {
            printf("Disconnected or error.\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}
