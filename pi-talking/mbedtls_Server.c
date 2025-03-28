// Server for receiving AES-GCM encrypted messages after PQC key exchange (Kyber, BIKE, HQC)
// Compile with:
// gcc mbedtls_Server.c -o mbed_server -lmbedtls -lmbedx509 -lmbedcrypto -loqs -lssl -lcrypto -L/usr/local/lib

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <oqs/oqs.h>
#include <mbedtls/gcm.h>
#include <openssl/sha.h>  //
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 2048
#define AES_KEY_SIZE 32
#define AES_IV_SIZE 12
#define AES_TAG_SIZE 16

// Function to receive encrypted message from client
int recv_encrypted_message(int client_socket, uint8_t *iv, uint8_t *ciphertext, size_t *cipher_len, uint8_t *tag) {
    if (recv(client_socket, iv, AES_IV_SIZE, 0) != AES_IV_SIZE) return -1;
    if (recv(client_socket, tag, AES_TAG_SIZE, 0) != AES_TAG_SIZE) return -1;

    int bytes_received = recv(client_socket, ciphertext, BUFFER_SIZE, 0);
    if (bytes_received <= 0) return -1;

    *cipher_len = bytes_received;
    return 0;
}

// AES-GCM decryption function using mbedTLS
int aes_gcm_decrypt(uint8_t *ciphertext, size_t cipher_len, uint8_t *aes_key, uint8_t *iv, uint8_t *plaintext, uint8_t *tag) {
    mbedtls_gcm_context gcm;
    mbedtls_gcm_init(&gcm);

    if (mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, aes_key, AES_KEY_SIZE * 8) != 0) {
        printf("[ERROR] Failed to set AES-GCM key!\n");
        mbedtls_gcm_free(&gcm);
        return -1;
    }

    int ret = mbedtls_gcm_auth_decrypt(&gcm, cipher_len, iv, AES_IV_SIZE, NULL, 0, tag, AES_TAG_SIZE, ciphertext, plaintext);
    mbedtls_gcm_free(&gcm);

    if (ret != 0) {
        printf("[SERVER] AES-GCM decryption failed!\n");
        return -1;
    }

    return (int)cipher_len;
}

void handle_client(int client_socket) {
    printf("[SERVER] Client connected. Performing key exchange...\n");

    // Select your KEM here
    //KEM
    // OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_bike_l1);
    // OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_bike_l3);
    // OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_bike_l5);

    //kyber
    // OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_kyber_512);
    // OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_kyber_768);
    // OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_kyber_1024);

    //HQC
    // OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_hqc_128);
    // OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_hqc_192);
    // OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_hqc_256);




    if (!kem) {
        printf("[ERROR] Failed to initialize KEM!\n");
        close(client_socket);
        return;
    }

    uint8_t *public_key = malloc(kem->length_public_key);
    uint8_t *secret_key = malloc(kem->length_secret_key);
    uint8_t *ciphertext = malloc(kem->length_ciphertext);
    uint8_t *shared_secret = malloc(kem->length_shared_secret);

    if (!public_key || !secret_key || !ciphertext || !shared_secret) {
        printf("[ERROR] Memory allocation failed!\n");
        goto cleanup;
    }

    if (OQS_KEM_keypair(kem, public_key, secret_key) != OQS_SUCCESS) {
        printf("[ERROR] Failed to generate key pair!\n");
        goto cleanup;
    }

    if (send(client_socket, public_key, kem->length_public_key, 0) != (int)kem->length_public_key) {
        printf("[ERROR] Failed to send public key!\n");
        goto cleanup;
    }

    if (recv(client_socket, ciphertext, kem->length_ciphertext, 0) != (int)kem->length_ciphertext) {
        printf("[ERROR] Failed to receive ciphertext!\n");
        goto cleanup;
    }

    if (OQS_KEM_decaps(kem, shared_secret, ciphertext, secret_key) != OQS_SUCCESS) {
        printf("[ERROR] Key decapsulation failed!\n");
        goto cleanup;
    }

    printf("[SERVER] Key exchange complete! Shared secret established.\n");

    // ✅ Derive AES-256 key from shared secret using SHA-256
    uint8_t aes_key[32];
    SHA256(shared_secret, kem->length_shared_secret, aes_key);

    while (1) {
        uint8_t iv[AES_IV_SIZE];
        uint8_t encrypted_msg[BUFFER_SIZE];
        uint8_t tag[AES_TAG_SIZE];
        size_t encrypted_len;

        if (recv_encrypted_message(client_socket, iv, encrypted_msg, &encrypted_len, tag) < 0) {
            perror("[SERVER] Failed to receive encrypted message");
            break;
        }

        printf("[SERVER] Received encrypted message of length: %zu bytes\n", encrypted_len);
        printf("[SERVER] Encrypted message (hex): ");
        for (size_t i = 0; i < encrypted_len; i++) {
            printf("%02X ", encrypted_msg[i]);
        }
        printf("\n");

        uint8_t decrypted_msg[BUFFER_SIZE] = {0};
        int decrypted_len = aes_gcm_decrypt(encrypted_msg, encrypted_len, aes_key, iv, decrypted_msg, tag);

        if (decrypted_len < 0) {
            printf("[SERVER] AES-GCM decryption failed!\n");
        } else {
            printf("[SERVER] Decrypted message: %s\n", decrypted_msg);
            printf("[SERVER] Decrypted message length: %d bytes\n", decrypted_len);
        }
    }

cleanup:
    OQS_MEM_secure_free(public_key, kem->length_public_key);
    OQS_MEM_secure_free(secret_key, kem->length_secret_key);
    OQS_MEM_secure_free(ciphertext, kem->length_ciphertext);
    OQS_MEM_secure_free(shared_secret, kem->length_shared_secret);
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
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.101"); // Your Pi IP
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(1);
    }

    listen(server_socket, 5);
    printf("[SERVER] Listening on port %d...\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
        if (client_socket < 0) {
            perror("Client accept failed");
            continue;
        }

        printf("[SERVER] Accepted a new client connection.\n");

        if (fork() == 0) {
            close(server_socket);
            handle_client(client_socket);
            close(client_socket);
            exit(0);
        }

        close(client_socket);
    }

    close(server_socket);
    return 0;
}
