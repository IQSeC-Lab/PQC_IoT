// SERVER
// Developed by Jesus Lopez
// server.c
// gcc client.c -o chat_client.o -lmbedtls -lmbedx509 -lmbedcrypto -loqs -lssl -lcrypto -L/usr/local/lib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <oqs/oqs.h>
#include <time.h>
#include <mbedtls/gcm.h>
#include <openssl/rand.h>
#include <openssl/sha.h>


#define PORT 8080
#define BUFFER_SIZE 2048
#define AES_KEY_SIZE 32
#define AES_IV_SIZE 12
#define AES_TAG_SIZE 16

int send_encrypted_message(int client_socket, uint8_t *iv, uint8_t *ciphertext, size_t cipher_len, uint8_t *tag) {
    if (send(client_socket, iv, AES_IV_SIZE, 0) != AES_IV_SIZE) return -1;
    if (send(client_socket, tag, AES_TAG_SIZE, 0) != AES_TAG_SIZE) return -1;
    if (send(client_socket, ciphertext, cipher_len, 0) != (int)cipher_len) return -1;
    return 0;
}

int aes_gcm_encrypt(const unsigned char *plaintext, size_t len, const unsigned char *aes_key,
                    unsigned char *iv, unsigned char *ciphertext, unsigned char *tag) {
    mbedtls_gcm_context gcm;
    mbedtls_gcm_init(&gcm);

    if (mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, aes_key, AES_KEY_SIZE * 8) != 0) {
        printf("[ERROR] Failed to set AES-GCM key!\n");
        mbedtls_gcm_free(&gcm);
        return -1;
    }

    if (RAND_bytes(iv, AES_IV_SIZE) != 1) {
        printf("[ERROR] Failed to generate IV!\n");
        mbedtls_gcm_free(&gcm);
        return -1;
    }

    int ret = mbedtls_gcm_crypt_and_tag(&gcm, MBEDTLS_GCM_ENCRYPT, len,
        iv, AES_IV_SIZE, NULL, 0, plaintext, ciphertext, AES_TAG_SIZE, tag);

    mbedtls_gcm_free(&gcm);
    return (ret == 0) ? (int)len : -1;
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    uint8_t iv[AES_IV_SIZE];
    char server_ip[32];
    printf("Enter the server IP address: ");
    fgets(server_ip, sizeof(server_ip), stdin);
    server_ip[strcspn(server_ip, "\n")] = 0; // Remove newline


    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("[ERROR] Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("[ERROR] Connection failed");
        exit(1);
    }

    printf("[CLIENT] Connected to chat. Performing key exchange...\n");
    OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_kyber_1024);  // or bike_l5, hqc_256
    if (!kem) {
        printf("[ERROR] Failed to initialize KEM!\n");
        exit(1);
    }

    uint8_t *public_key = malloc(kem->length_public_key);
    uint8_t *ciphertext = malloc(kem->length_ciphertext);
    uint8_t *shared_secret = malloc(kem->length_shared_secret);

    if (recv(client_socket, public_key, kem->length_public_key, 0) != (int)kem->length_public_key) {
        printf("[ERROR] Failed to receive public key from server!\n");
        goto cleanup;
    }

    if (OQS_KEM_encaps(kem, ciphertext, shared_secret, public_key) != OQS_SUCCESS) {
        printf("[ERROR] KEM encapsulation failed!\n");
        goto cleanup;
    }

    if (send(client_socket, ciphertext, kem->length_ciphertext, 0) != (int)kem->length_ciphertext) {
        printf("[ERROR] Failed to send ciphertext to server!\n");
        goto cleanup;
    }

    printf("[CLIENT] Key exchange complete. Shared secret established.\n");

    // Derive AES key
    uint8_t aes_key[32];
    SHA256(shared_secret, kem->length_shared_secret, aes_key);

    // 🔐 Send name encrypted
    char name[64];
    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; // remove newline

    uint8_t name_iv[AES_IV_SIZE], name_cipher[BUFFER_SIZE], name_tag[AES_TAG_SIZE];
    int name_len = aes_gcm_encrypt((unsigned char*)name, strlen(name), aes_key, name_iv, name_cipher, name_tag);
    send_encrypted_message(client_socket, name_iv, name_cipher, name_len, name_tag);

    // 💬 Main chat loop
    while (1) {
        char input[BUFFER_SIZE];
        printf("You: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "/exit") == 0) {
            printf("[CLIENT] Exiting...\n");
            break;
        }

        uint8_t enc_msg[BUFFER_SIZE], tag[AES_TAG_SIZE];
        int enc_len = aes_gcm_encrypt((unsigned char*)input, strlen(input), aes_key, iv, enc_msg, tag);
        if (enc_len < 0) break;

        if (send_encrypted_message(client_socket, iv, enc_msg, enc_len, tag) < 0) {
            perror("[CLIENT] Failed to send message");
            break;
        }
    }

cleanup:
    OQS_MEM_secure_free(public_key, kem->length_public_key);
    OQS_MEM_secure_free(ciphertext, kem->length_ciphertext);
    OQS_MEM_secure_free(shared_secret, kem->length_shared_secret);
    OQS_KEM_free(kem);
    close(client_socket);
    return 0;
}
