#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <oqs/oqs.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    printf("[SERVER] Client connected. Performing key exchange with BIKE-L1...\n");

    OQS_KEM *kem = OQS_KEM_new("BIKE-L1");
    if (!kem) {
        printf("Error initializing BIKE-L1\n");
        close(client_socket);
        return;
    }

    // Generate key pair
    uint8_t public_key[kem->length_public_key];
    uint8_t secret_key[kem->length_secret_key];
    OQS_KEM_keypair(kem, public_key, secret_key);

    // Send public key to client
    send(client_socket, public_key, kem->length_public_key, 0);

    // Receive encapsulated secret from client
    uint8_t ciphertext[kem->length_ciphertext];
    recv(client_socket, ciphertext, kem->length_ciphertext, 0);

    // Decapsulate shared secret
    uint8_t shared_secret[kem->length_shared_secret];
    OQS_KEM_decaps(kem, shared_secret, ciphertext, secret_key);
    printf("[SERVER] Key exchange complete! Shared secret established.\n");

    // Receive encrypted message
    uint8_t encrypted_msg[BUFFER_SIZE];
    recv(client_socket, encrypted_msg, BUFFER_SIZE, 0);

    // Print received message (in a real application, you'd decrypt it)
    printf("[SERVER] Received encrypted message: %s\n", encrypted_msg);

    // Clean up
    OQS_KEM_free(kem);
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(server_socket, 5);
    printf("[SERVER] Listening on port %d...\n", PORT);

    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
    if (client_socket < 0) {
        perror("Client accept failed");
        exit(1);
    }

    handle_client(client_socket);
    close(server_socket);

    return 0;
}