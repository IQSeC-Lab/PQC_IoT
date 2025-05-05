// SERVER
// Developed by Jesus Lopez
// server.c
// gcc -o pqc_server server.c -loqs -lmbedcrypto -lmbedtls -lmbedx509 -lcrypto -L/usr/local/lib

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <oqs/oqs.h>
#include <mbedtls/gcm.h>
#include <openssl/sha.h>
#include <time.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 2048
#define AES_KEY_SIZE 32
#define AES_IV_SIZE 12
#define AES_TAG_SIZE 16
#define MAX_NAME_LEN 32

// New data structure for the client
typedef struct {
    int socket;
    uint8_t aes_key[AES_KEY_SIZE];
    char name[MAX_NAME_LEN];
} Client;

Client clients[MAX_CLIENTS];

void generate_iv(uint8_t *iv) {
    for (int i = 0; i < AES_IV_SIZE; i++)
        iv[i] = rand() % 256;
}

int recv_all(int sock, uint8_t *buffer, size_t length) {
    size_t total = 0;
    while (total < length) {
        ssize_t received = recv(sock, buffer + total, length - total, 0);
        if (received <= 0) return -1;
        total += received;
    }
    return 0;
}

int aes_gcm_decrypt(uint8_t *ciphertext, size_t cipher_len, uint8_t *aes_key, uint8_t *iv, uint8_t *plaintext, uint8_t *tag) {
    mbedtls_gcm_context gcm;
    mbedtls_gcm_init(&gcm);
    mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, aes_key, AES_KEY_SIZE * 8);
    int ret = mbedtls_gcm_auth_decrypt(&gcm, cipher_len, iv, AES_IV_SIZE, NULL, 0, tag, AES_TAG_SIZE, ciphertext, plaintext);
    mbedtls_gcm_free(&gcm);
    return (ret == 0) ? (int)cipher_len : -1;
}

int aes_gcm_encrypt(uint8_t *plaintext, size_t plain_len, uint8_t *aes_key, uint8_t *iv, uint8_t *ciphertext, uint8_t *tag) {
    mbedtls_gcm_context gcm;
    mbedtls_gcm_init(&gcm);
    mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, aes_key, AES_KEY_SIZE * 8);
    int ret = mbedtls_gcm_crypt_and_tag(&gcm, MBEDTLS_GCM_ENCRYPT, plain_len, iv, AES_IV_SIZE, NULL, 0, plaintext, ciphertext, AES_TAG_SIZE, tag);
    mbedtls_gcm_free(&gcm);
    return (ret == 0) ? (int)plain_len : -1;
}

void broadcast_to_others(int sender_index, uint8_t *plaintext, size_t length) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (i == sender_index || clients[i].socket == 0) continue;

        uint8_t iv[AES_IV_SIZE], tag[AES_TAG_SIZE], ciphertext[BUFFER_SIZE];
        generate_iv(iv);

        if (aes_gcm_encrypt(plaintext, length, clients[i].aes_key, iv, ciphertext, tag) < 0) {
            printf("[ERROR] Encryption failed for client %d\n", i);
            continue;
        }

        send(clients[i].socket, iv, AES_IV_SIZE, 0);
        send(clients[i].socket, tag, AES_TAG_SIZE, 0);
        send(clients[i].socket, ciphertext, length, 0);
    }
}

void pqc_key_exchange(int client_socket, uint8_t *aes_key) {
    OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_kyber_1024); // Here change the PQC algorithm
    uint8_t *pk = malloc(kem->length_public_key);
    uint8_t *sk = malloc(kem->length_secret_key);
    uint8_t *ct = malloc(kem->length_ciphertext);
    uint8_t *ss = malloc(kem->length_shared_secret);

    OQS_KEM_keypair(kem, pk, sk);
    send(client_socket, pk, kem->length_public_key, 0);
    recv_all(client_socket, ct, kem->length_ciphertext);
    OQS_KEM_decaps(kem, ss, ct, sk);
    SHA256(ss, kem->length_shared_secret, aes_key);

    OQS_MEM_secure_free(pk, kem->length_public_key);
    OQS_MEM_secure_free(sk, kem->length_secret_key);
    OQS_MEM_secure_free(ct, kem->length_ciphertext);
    OQS_MEM_secure_free(ss, kem->length_shared_secret);
    OQS_KEM_free(kem);
}

/**
 * 
 * This is the main function for the server.
 */

int main() {
    srand(time(NULL));
    memset(clients, 0, sizeof(clients));

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr("192.168.0.4"), //Here is the address for the server
        .sin_port = htons(PORT)
    };
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    printf("[SERVER] Listening on port %d...\n", PORT);

    fd_set read_fds;
    int max_fd;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);
        max_fd = server_socket;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].socket > 0) {
                FD_SET(clients[i].socket, &read_fds);
                if (clients[i].socket > max_fd) max_fd = clients[i].socket;
            }
        }

        select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        
       // Check if there is a new connection
if (FD_ISSET(server_socket, &read_fds)) {
    int new_socket = accept(server_socket, NULL, NULL);
    int i;
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket == 0) {
            clients[i].socket = new_socket;

            // 1. Key Exchange
            pqc_key_exchange(new_socket, clients[i].aes_key);

            // 2. Receive Encrypted Name
            uint8_t iv[AES_IV_SIZE], tag[AES_TAG_SIZE];
            uint8_t encrypted_name[BUFFER_SIZE], decrypted_name[BUFFER_SIZE];
            size_t name_len;

            if (recv_all(new_socket, iv, AES_IV_SIZE) != 0 ||
                recv_all(new_socket, tag, AES_TAG_SIZE) != 0) {
                printf("[SERVER] Failed to receive client name metadata\n");
                close(new_socket);
                clients[i].socket = 0;
                break;
            }

            name_len = recv(new_socket, encrypted_name, BUFFER_SIZE, 0);
            if (name_len <= 0) {
                printf("[SERVER] Failed to receive encrypted client name\n");
                close(new_socket);
                clients[i].socket = 0;
                break;
            }

            int decrypted_len = aes_gcm_decrypt(encrypted_name, name_len, clients[i].aes_key, iv, decrypted_name, tag);
            if (decrypted_len > 0) {
                decrypted_name[decrypted_len] = '\0';
                strncpy(clients[i].name, (char*)decrypted_name, MAX_NAME_LEN);
                printf("[SERVER] Client %d connected with name: %s\n", new_socket, clients[i].name);
            } else {
                strcpy(clients[i].name, "UNKNOWN");
                printf("[SERVER] Failed to decrypt client name. Marked as UNKNOWN\n");
            }

            break;  // done handling new connection
        }
    }
} 

        

        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sock = clients[i].socket;
            if (sock > 0 && FD_ISSET(sock, &read_fds)) {
                uint8_t iv[AES_IV_SIZE], tag[AES_TAG_SIZE], encrypted[BUFFER_SIZE], decrypted[BUFFER_SIZE];
                size_t cipher_len;

                if (recv_all(sock, iv, AES_IV_SIZE) != 0 ||
                    recv_all(sock, tag, AES_TAG_SIZE) != 0) {
                    printf("[SERVER] Client %d disconnected.\n", sock);
                    close(sock);
                    clients[i].socket = 0;
                    continue;
                }

                cipher_len = recv(sock, encrypted, BUFFER_SIZE, 0);
                if (cipher_len <= 0) {
                    printf("[SERVER] Failed to receive ciphertext from %d\n", sock);
                    close(sock);
                    clients[i].socket = 0;
                    continue;
                }

                int decrypted_len = aes_gcm_decrypt(encrypted, cipher_len, clients[i].aes_key, iv, decrypted, tag);
                if (decrypted_len > 0) {
                    decrypted[decrypted_len] = '\0';  // null-terminate string
                    printf("[%s]: %s\n", clients[i].name, decrypted);
                    broadcast_to_others(i, decrypted, decrypted_len);
                } else {
                    printf("[SERVER] Failed to decrypt message from client %d\n", sock);
                }
            }
        }
    }

    close(server_socket);
    return 0;
}