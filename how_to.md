# This is an updated guide on how to install liboqs.

## Getting started

1. first update your system

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

## Installation

1. run this in the terminal

```
git clone --depth=1 https://github.com/open-quantum-safe/liboqs
cmake -S liboqs -B liboqs/build -DBUILD_SHARED_LIBS=ON
cmake --build liboqs/build --parallel 8
sudo cmake --build liboqs/build --target install
```

2. put this line on terminal if using linux (which is recommended)

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
```

3. lastly

```
export OQS_INSTALL_PATH=/path/to/liboqs
```

Remember to change the path to liboqs

## Alternate instalation

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

### How to use it in stand alone applications

1. import the library

```
import oqs
```
