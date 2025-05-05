# Post-Quantum Cryptography on IoT Devices

This research project evaluates the performance of post-quantum cryptographic algorithms on resource-constrained hardware. Our experiments were conducted on two Raspberry Pi units to benchmark key operations.

## Project Structure

- '1-Sockets-Implementations':
- 'chatroom': Additional Implementation

## Installation

We've included precompiled binaries in this repository. To build and run the code, you must install the following dependencies:

- **mbedTLS**
- **liboqs**

For step-by-step installation instructions—covering both the C libraries and the Python wrapper—see the `tutorial` directory. The `how_to.md` file walks you through:

1. Cloning and building the C libraries
2. Installing the Python bindings (if desired)

## Experimental Results

All measurement data is stored in the `data` folder. You will find:

- CSV files with raw benchmarking results
- Formatted tables summarizing key performance metrics

## Additional

We added an additional implementation of our code. We implemented a chatroom. **If interested**, please refer to the folder **7-chatroom**.

## Contributors

- **Jesus R. Lopez** (NotSamus) jlopez126@miners.utep.edu
- **Viviana Cadena** (Vixxi)
