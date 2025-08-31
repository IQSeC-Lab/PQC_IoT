# Post-Quantum Cryptography on IoT Devices

This research project evaluates the performance of post-quantum cryptographic algorithms on resource-constrained hardware. Our experiments were conducted on two Raspberry Pi units to benchmark key operations.


**This Paper is accepted to IEEE International Conference on Quantum Computing and Engineering (QCE25)**



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

## Acknowledgments

Jesus Lopez is funded by the Department of Computer Science at UTEP. Viviana Cadena is funded by the Cyber Halo Innovation Research Program (CHIRP) at Pacific Northwest National Laboratory (PNNL).


## Contributors
Any questions about the project or if you want to contribute, please reach out!!
- **Jesus R. Lopez** (NotSamus) jlopez126@miners.utep.edu
- **Viviana Cadena** (Vixxi) - vcadena1@miners.utep.edu


## Citation 
```
@misc{lopez2025evaluatingpostquantumcryptographicalgorithms,
      title={Evaluating Post-Quantum Cryptographic Algorithms on Resource-Constrained Devices}, 
      author={Jesus Lopez and Viviana Cadena and Mohammad Saidur Rahman},
      year={2025},
      eprint={2507.08312},
      archivePrefix={arXiv},
      primaryClass={cs.CR},
      url={https://arxiv.org/abs/2507.08312}, 
}
```
