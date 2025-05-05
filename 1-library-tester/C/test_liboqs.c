// gcc test_liboqs.c -o test_liboqs -loqs
#include <stdio.h>
#include <string.h>
#include <oqs/oqs.h>

int main() {
    printf("=== Testing liboqs Installation ===\n");

    // Choose a KEM algorithm (Kyber-768)
    const char *kem_name = "BIKE-L1";
    OQS_KEM *kem = OQS_KEM_new(kem_name);

    if (!kem) {
        printf("Error: KEM %s not found.\n", kem_name);
        return 1;
    }

    printf("\nTesting KEM: %s\n", kem->method_name);

    uint8_t public_key[kem->length_public_key];
    uint8_t secret_key[kem->length_secret_key];
    uint8_t ciphertext[kem->length_ciphertext];
    uint8_t shared_secret_encap[kem->length_shared_secret];
    uint8_t shared_secret_decap[kem->length_shared_secret];

    // Generate key pair
    if (OQS_KEM_keypair(kem, public_key, secret_key) != OQS_SUCCESS) {
        printf("Error: Keypair generation failed!\n");
        return 1;
    }

    // Encapsulate shared secret
    if (OQS_KEM_encaps(kem, ciphertext, shared_secret_encap, public_key) != OQS_SUCCESS) {
        printf("Error: Encapsulation failed!\n");
        return 1;
    }

    // **FIXED: Correct function call**
    if (OQS_KEM_decaps(kem, shared_secret_decap, ciphertext, secret_key) != OQS_SUCCESS) {
        printf("Error: Decapsulation failed!\n");
        return 1;
    }

    // Compare shared secrets
    if (memcmp(shared_secret_encap, shared_secret_decap, kem->length_shared_secret) == 0) {
        printf("Success! Shared secrets match!\n");
    } else {
        printf("Error: Shared secrets do NOT match!\n");
    }

    // Clean up
    OQS_KEM_free(kem);
    printf("\n=== liboqs test completed! ===\n");

    return 0;
}