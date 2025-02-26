#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <oqs/oqs.h>
#include <mbedtls/gcm.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define AES_KEY_SIZE 32  // 256-bit key
#define AES_IV_SIZE 12   // 96-bit IV for GCM

// AES-GCM decryption function using mbedTLS
int aes_gcm_decrypt(uint8_t *ciphertext, int ciphertext_len, uint8_t *key, uint8_t *iv, uint8_t *plaintext, uint8_t *tag) {
    mbedtls_gcm_context gcm;
    mbedtls_gcm_init(&gcm);
    
    if (mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, key, AES_KEY_SIZE * 8) != 0) {
        mbedtls_gcm_free(&gcm);
        return -1;
    }

    if (mbedtls_gcm_auth_decrypt(&gcm, ciphertext_len, iv, AES_IV_SIZE, NULL, 0, tag, 16, ciphertext, plaintext) != 0) {
        mbedtls_gcm_free(&gcm);
        return -1;
    }

    mbedtls_gcm_free(&gcm);
    return ciphertext_len;
}

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

    // Receive IV, encrypted message, and tag
    uint8_t iv[AES_IV_SIZE];
    uint8_t encrypted_msg[BUFFER_SIZE];
    uint8_t tag[16];
    recv(client_socket, iv, AES_IV_SIZE, 0);
    recv(client_socket, encrypted_msg, BUFFER_SIZE, 0);
    recv(client_socket, tag, 16, 0);

    // Decrypt message with AES-GCM
    uint8_t decrypted_msg[BUFFER_SIZE] = {0};
    int decrypted_len = aes_gcm_decrypt(encrypted_msg, BUFFER_SIZE, shared_secret, iv, decrypted_msg, tag);

    if (decrypted_len < 0) {
        printf("[SERVER] AES-GCM decryption failed!\n");
    } else {
        printf("[SERVER] Decrypted message: %s\n", decrypted_msg);
    }

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

