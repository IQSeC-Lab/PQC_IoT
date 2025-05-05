# Sockets Implementation

This is an implementation of both libraries, It runs locally on the terminal. This code takes as an input a file named `networkSim.txt` This file will be sent from the client to the server.

## Compile code

Use this to compile the code on this folder

```
gcc -o server mbedtls_Server.c -loqs -lmbedtls -lmbedx509 -lmbedcrypto -lcrypto -L/usr/local/lib
```

```
gcc mbedtls_Client.c -o mbed_client -lmbedtls -lmbedx509 -lmbedcrypto -loqs -lssl -lcrypto -L/usr/local/lib
```
