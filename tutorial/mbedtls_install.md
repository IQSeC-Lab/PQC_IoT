# Installing mbedtls

1. First, Install the dependencies necessaries for mbedtls:
*Note:* Make sure that jinja2, jsonschema, is installed and that everything is updated.

```
sudo apt update && sudo apt install -y python3-jinja2 python3-jsonschema
```

2. Then clone the repo of mbedtls into your desired folder:
```
git clone https://github.com/Mbed-TLS/mbedtls.git
```

```
cd mbedtls
cmake ..
```

**Note**: if cmake returns an error saying that the CMakeList.txt file is not found Run the following command:
```
git submodule update --init or git submodule update --init  --recursive
rm -rf build
mkdir build
cd build
cmake ..
```
Note: Make sure that you cd to the mbedtls directory to run the following command
```
make -j$(nproc)
sudo make install
```

When compiling code with mbedtls use:
gcc my_program.c -o my_program -lmbedtls -lmbedx509 -lmbedcrypto

