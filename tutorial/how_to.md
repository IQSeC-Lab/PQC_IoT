# This is an updated guide on how to install liboqs.

There are a couple ways to install liboqs here we are going to layout some of them:

- liboqs (The main implementation in c)
- liboqs-python (The Python wrapper)

## Getting started

These are going to be required across almost all the implementations of liboqs:

1. First update your system

```
sudo apt update -y
sudo apt upgrade -y
```

2. Then install make sure you have installed openssl

```
openssl version
```

3. also install this

```
sudo apt install astyle cmake gcc ninja-build libssl-dev python3-pytest python3-pytest-xdist unzip xsltproc doxygen graphviz python3-yaml valgrind
```

# Installation of C Implementation

1. run this in the terminal

```
git clone --depth=1 https://github.com/open-quantum-safe/liboqs
cd ~/Desktop/liboqs
mkdir -p build && cd build
cmake .. -G Ninja \ 
    -DBUILD_SHARED_LIBS=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DOQS_ALGS_ENABLED=All \
    -DOQS_ENABLE_KEM_ALG=ON \
    -DOQS_ENABLE_SIG_ALG=ON \
    -DOQS_ENABLE_SIG_STFL_ALG=ON \
    -DOQS_BUILD_ONLY_LIB=OFF \
    -DOQS_DIST_BUILD=ON \
    -DOQS_USE_OPENSSL=ON \
    -DOQS_ENABLE_TEST_CONSTANT_TIME=ON \
    -DOQS_OPT_TARGET=auto \
    -DOQS_SPEED_USE_ARM_PMU=OFF \
    -DUSE_SANITIZER=Address \
    -DOQS_ENABLE_LIBJADE_KEM_ALG=ON \
    -DOQS_ENABLE_LIBJADE_SIG_ALG=ON \
    -DOQS_BUILD_FUZZ_TESTS=ON
ninja
sudo ninja install
```

**Disclaimer:** on the second line of _cd_ please make sure to replace the folder where you have cloned the liboqs

2. put this line on terminal if using linux (which is recommended)

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
```

3. lastly

```
export OQS_INSTALL_PATH=/path/to/liboqs
```

or
if using bash, do this to make it permanent

```
echo 'export LD_LIBRARY_PATH=~/Desktop/liboqs/build:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc
```

Remember to change the path to liboqs

## How to use

The next code snippet is a sample code to test the library installation:

```c
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

    // ✅ FIX: Use struct members instead of function calls
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
```

To compile:

```
gcc test_liboqs.c -o test_liboqs \
    -I~/Desktop/liboqs/include \
    -L~/Desktop/liboqs/build -loqs \
    -Wl,-rpath,~/Desktop/liboqs/build
```

**Disclaimer**: please make sure to change to the correct folder.

# Installation of the Python wrapper

1. install venv

```
sudo apt install python3-venv -y
```

2. Then run the following command

```
python3 -m venv venv
. venv/bin/activate
python3 -m ensurepip --upgrade
```

then:

```
. venv/bin/activate
```

3. then is configure and install the wrapper

```
git clone --depth=1 https://github.com/open-quantum-safe/liboqs-python
cd liboqs-python
pip install .
```

4. Try to run the examples

```
python3 liboqs-python/examples/kem.py
python3 liboqs-python/examples/sig.py
python3 liboqs-python/examples/rand.py
```

_side note:_ the interesting part of using the wrapper is that if liboqs is not detected at runtime, it will be downloaded, configured and installed automatically as a shared library. and this will happen only when loading the wrapper.

## How to use

The following code is to test the implementation in python:

```Python
import oqs

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
            print("\u2705 Shared secret successfully established!")
        else:
            print("\u274c Shared secret mismatch!")

if __name__ == "__main__":
    test_oqs()
```

**note**, if running the code, please make sure you are in super user mode, you can do this by:

```
sudo su
```

and also do not forger to activate the venv:

```
. venv/bin/activate
```
