# Code Documentation and Explanation
This document will server as a explanation of what each part of the code does and what its purpose is.

The code contains the following #define statements that are used to define constants. These are in both the server and client code.

```C
#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 2048   // 2KB for messages
#define AES_KEY_SIZE 32    // 256-bit AES key
#define AES_IV_SIZE 12     // 96-bit IV for GCM
#define AES_TAG_SIZE 16    // AES-GCM tag
```

**SERVER_IP and PORT:** Define the server's IP address and port number for network communication.

**BUFFER_SIZE:** Sets the size of the buffer for messages, which is 2048 bytes (2KB).

**AES_KEY_SIZE:** Defines the size of the AES key as 32 bytes (256 bits), which is typical for AES-256 encryption.

**AES_IV_SIZE:** Specifies the size of the IV (Initialization Vector) as 12 bytes (96 bits), which is standard for AES-GCM.

The Initialization Vector is a random value used in symmetric encryption algorithms to ensure that the same plaintext, encrypted with the same key, produces a new ciphertext every time.
Here we are using OpenSSL's Rand_Byte to generate secure random IV's.

**AES_TAG_SIZE:** Defines the size of the authentication tag as 16 bytes (128 bits), ensuring integrity in AES-GCM encryption

The authentication tag is used to ensure data integrity and authenticity by verifying that the ciphertext hasn't been tampered with during transmission or storage.

##
## What is AES-GCM?
AES-GCM or Advanced Encryption Standard - Galois/Counter Mode is a widely used cryptographic algorithm that provides both confidentiality (authenticated encryption) and authentication (integrity verification) for data. It combines AES with GCM mode of operation, which uses a counter for encryption and Galois field multiplication for authentication. It has good performance, is highly optimized, FIPS-approved, and well-trusted.

GCM has two operations, authenticated encryption and authenticated decryption.

There are four inputs for authenticated encryption:
1. Secret key
2. Initialization vector (IV) (sometimes called a nonce†)
3. The plaintext itself 
4. Optional additional authentication data (AAD)

The nonce and AAD are passed in the clear.

There are two outputs: 
1. The ciphertext, which is exactly the same length as the plaintext.
2. An authentication tag (the "tag"). 
Sometimes called the message authentication code (MAC) or integrity check value (ICV).


https://csrc.nist.gov/csrc/media/Events/2023/third-workshop-on-block-cipher-modes-of-operation/documents/accepted-papers/Practical%20Challenges%20with%20AES-GCM.pdf

https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=dd2166539a6ed65fca90e251f8a9af26e7bf71c2