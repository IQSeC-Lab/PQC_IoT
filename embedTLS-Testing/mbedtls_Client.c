#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <oqs/oqs.h>
#include <mbedtls/gcm.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024
#define AES_KEY_SIZE 32  // 256-bit key
#define AES_IV_SIZE 12   // 96-bit IV for GCM

// AES-GCM encryption function using mbedTLS
int aes_gcm_encrypt(uint8_t *plaintext, int plaintext_len, uint8_t *key, uint8_t *iv, uint8_t *ciphertext, uint8_t *tag) {
    mbedtls_gcm_context gcm;
    mbedtls_gcm_init(&gcm);
    
    if (mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, key, AES_KEY_SIZE * 8) != 0) {
        mbedtls_gcm_free(&gcm);
        return -1;
    }

    if (mbedtls_gcm_crypt_and_tag(&gcm, MBEDTLS_GCM_ENCRYPT, plaintext_len, iv, AES_IV_SIZE, NULL, 0, plaintext, ciphertext, 16, tag) != 0) {
        mbedtls_gcm_free(&gcm);
        return -1;
    }

    mbedtls_gcm_free(&gcm);
    return plaintext_len;
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    uint8_t message[] = "Hello, PQC-secured World with BIKE-L1!";
    uint8_t iv[AES_IV_SIZE] = {0};  // Simple IV for testing (should be random)

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

    printf("[CLIENT] Key exchange complete! Encrypting message with AES-GCM using mbedTLS...\n");

    // Encrypt message with AES-GCM
    uint8_t encrypted_msg[BUFFER_SIZE];
    uint8_t tag[16];
    int encrypted_len = aes_gcm_encrypt(message, strlen((char*)message), shared_secret, iv, encrypted_msg, tag);
    
    if (encrypted_len < 0) {
        printf("[CLIENT] AES-GCM encryption failed!\n");
        exit(1);
    }

    // Send IV, encrypted message, and tag
    send(client_socket, iv, AES_IV_SIZE, 0);
    send(client_socket, encrypted_msg, encrypted_len, 0);
    send(client_socket, tag, 16, 0);

    printf("[CLIENT] Encrypted message sent!\n");

    // Clean up
    OQS_KEM_free(kem);
    close(client_socket);

    return 0;
}
