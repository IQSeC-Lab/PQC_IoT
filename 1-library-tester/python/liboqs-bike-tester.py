import oqs

"""
This script tests the OQS library with the BIKE-L1 algorithm.
"""
def test_oqs():
    algorithm = "BIKE-L1"  # You can change this to other PQC algorithms supported by OQS

    print(f"Testing {algorithm} with OQS...\n")

    # Create a Key Encapsulation Mechanism (KEM) instance
    with oqs.KeyEncapsulation(algorithm) as server:
        # Generate keypair
        public_key = server.generate_keypair()
        print(f"Public Key: {public_key.hex()}\n")

        # Simulate a client encapsulating a shared secret
        with oqs.KeyEncapsulation(algorithm) as client:
            ciphertext, shared_secret_client = client.encap_secret(public_key)
            print(f"Ciphertext: {ciphertext.hex()}\n")
            print(f"Shared Secret (Client): {shared_secret_client.hex()}\n")

        # Server decapsulates the shared secret
        shared_secret_server = server.decap_secret(ciphertext)
        print(f"Shared Secret (Server): {shared_secret_server.hex()}\n")

        # Verify the shared secret matches
        if shared_secret_client == shared_secret_server:
            print("✅ Shared secret successfully established!")
        else:
            print("❌ Shared secret mismatch!")

if __name__ == "__main__":
    test_oqs()
