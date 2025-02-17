#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <oqs/oqs.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    uint8_t message[] = "Hello, PQC-secured World with BIKE-L1!";

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("[CLIENT] Connected to server. Performing key exchange with BIKE-L1...\n");

    OQS_KEM *kem = OQS_KEM_new("BIKE-L1");
    if (!kem) {
        printf("Error initializing BIKE-L1\n");
        exit(1);
    }

    // Receive server's public key
    uint8_t public_key[kem->length_public_key];
    recv(client_socket, public_key, kem->length_public_key, 0);

    // Generate shared secret
    uint8_t ciphertext[kem->length_ciphertext];
    uint8_t shared_secret[kem->length_shared_secret];
    OQS_KEM_encaps(kem, ciphertext, shared_secret, public_key);

    // Send ciphertext to server
    send(client_socket, ciphertext, kem->length_ciphertext, 0);

    printf("[CLIENT] Key exchange complete! Sending encrypted message...\n");

    // Encrypt message (Here you would use AES-GCM with shared_secret)
    uint8_t encrypted_msg[BUFFER_SIZE];
    strcpy((char*)encrypted_msg, (char*)message); // Simulating encryption

    // Send encrypted message
    send(client_socket, encrypted_msg, BUFFER_SIZE, 0);

    printf("[CLIENT] Message sent!\n");

    // Clean up
    OQS_KEM_free(kem);
    close(client_socket);

    return 0;
}