# Installation of MbedTLS
Note: You must have OpenSSL already installed in your machine to be able to compile the code.
The client code uses OpenSSL to generate a random iv necessary for AES-GCM encryption. 

To check that you do have OpenSSL installed in your machine run:
```
openssl --version
```
or :
```
which openssl
```
This code uses OpenSSL 3.2.0 23 Nov 2023 (Library: OpenSSL 3.4.0 22 Oct 2024) version

To install OpenSSL in case you don't have it, run:
```
sudo apt update
sudo apt install openssl libssl-dev
```
Verify with:
```
ls /usr/include/openssl
```


## Installing mbedtls

1. First, install the necessary dependencies for mbedtls:

```
sudo apt update && sudo apt install -y python3-jinja2 python3-jsonschema
```

2. Then clone the repo of mbedtls into your desired folder:

```
git clone https://github.com/Mbed-TLS/mbedtls.git
```

```
cd mbedtls
git submodule update --init or git submodule update --init  --recursive //(either or is fine)
mkdir build
cd build
cmake ..
```

Then 

```
make -j$(nproc)
sudo make install
```
Afterwards install this:
```
sudo apt install libmbedtls-dev
```
To double check that the library was install run:
```
ldconfig -p | grep mbedcrypto
```
You should see some something like:
```
libmbedcrypto.so.7 (libc6,xxx) => /lib/xxx-linux-gnu/libmbedcrypto.so.7
libmbedcrypto.so (libc6,xxx) => /lib/xxx-linux-gnu/libmbedcrypto.so
```
Then:

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
```

When compiling code with mbedtls and openssl use:

```
// This is for the client
gcc mbedtls_Client.c -o mbed_client -lmbedtls -lmbedx509 -lmbedcrypto -loqs -lssl -lcrypto -L/usr/local/lib
```
