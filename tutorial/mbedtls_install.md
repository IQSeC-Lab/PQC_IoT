# Installing mbedtls

1. First, install the necessary dependencies for mbedtls:
   _Note:_ Make sure that jinja2, jsonschema, is installed and that everything is updated.

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

When compiling code with mbedtls use:

```
// This is for the client
gcc mbedtls_Client.c -o mbed_client -lmbedtls -lmbedx509 -lmbedcrypto -loqs
```
