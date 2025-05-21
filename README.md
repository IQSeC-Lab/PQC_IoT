# Post-Quantum Cryptography on IoT Devices

This research project evaluates the performance of post-quantum cryptographic algorithms on resource-constrained hardware. Our experiments were conducted on two Raspberry Pi units to benchmark key operations.

## Project Structure

- `1-Guides`: Provides guides on how to install all the dependencies needed.
- `1-library-tester`: Offers testers for the libraries required to work with this project.
- `2-Sockets-implementation`: Simple implementation using sockets.
- `3-embedtls-loqs`: Implementation of both libraries.
- `4-pi-talking`: Folder of the implementation with the two raspberry pi.
- `5-Results`: Raw data and graphs acquired during testing.
- `6-chatroom`: Additional implementation.
- `7-Docker`: Docker version for learning purposes

## Installation

We've included precompiled binaries in this repository. To build and run the code, you must install the following dependencies:

- **mbedTLS**
- **liboqs**

For step-by-step installation instructions—covering both the C libraries and the Python wrapper—see the `1-Guides` directory. The `loqs_installation_guide.md` and `mbedtls_installation_guide.md` files walks you through:

1. Cloning and building the C libraries
2. Installing the Python bindings (if desired)

## Experimental Results

All measurement data is located at the `5-Results` folder. You will find:

- CSV files with raw benchmarking results
- Formatted tables summarizing key performance metrics

## Additional

We added an additional implementations of our code, **if interested** refer to:
- `6-chatroom` -> chatroom using PQC and AES-GCM to communicate between two clients in a multithreaded server.
- `7-Docker` -> Learning implementation with hands-on.

## Contributors

- **Jesus R. Lopez** (NotSamus) jlopez126@miners.utep.edu
- **Viviana Cadena** (Vixxi) - not email
