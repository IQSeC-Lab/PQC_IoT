# ChatRoom Using Kyber

This module is an additional implementation developed for benchmarking purposes. It introduces a secure chatroom designed exclusively for wired connections between specific IP addresses.

The chat system operates by performing a Post-Quantum Cryptography (PQC) key exchange, after which all communication is encrypted using AES-GCM. The server is multi-threaded to efficiently handle multiple client connections simultaneously.

## Network Configuration

The chatroom will operate over the following static IP addresses:

- **Client A**: `192.168.1.101`
- **Client B**: `192.168.1.102`
- **Server**: `192.168.1.103`

## Compilation

Use the following command to compile for the `client`:

```
gcc client.c -o chat_client -lpthread -lmbedtls -lmbedx509 -lmbedcrypto -loqs -lssl -lcrypto -L/usr/local/lib
```

Use the command to compile for the `server`:

```
gcc -o pqc_server server.c -loqs -lmbedcrypto -lmbedtls -lmbedx509 -lcrypto -L/usr/local/lib
```
