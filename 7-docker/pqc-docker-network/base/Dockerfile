FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV OQS_INSTALL_PATH=/usr/local

# 1. Install all dependencies (for liboqs, mbedTLS, OpenSSL)
RUN apt-get update && apt-get install -y \
    build-essential cmake gcc git ninja-build curl unzip \
    libssl-dev openssl \
    python3-pytest python3-pytest-xdist \
    xsltproc doxygen graphviz python3-yaml \
    python3-jinja2 python3-jsonschema valgrind \
    libmbedtls-dev \
    iproute2 \
    neovim \
    vim \
    nano\
    iputils-ping

# 2. Build liboqs from source
WORKDIR /opt
RUN git clone --depth=1 https://github.com/open-quantum-safe/liboqs && \
    cd liboqs && mkdir build && cd build && \
    cmake .. -G Ninja \
        -DBUILD_SHARED_LIBS=ON \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=${OQS_INSTALL_PATH} \
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
        -DOQS_BUILD_FUZZ_TESTS=ON && \
    ninja && ninja install

# 3. Build mbedTLS from source
WORKDIR /opt
RUN git clone --depth=1 https://github.com/Mbed-TLS/mbedtls && \
    cd mbedtls && git submodule update --init --recursive && \
    mkdir build && cd build && \
    cmake .. && \
    make -j$(nproc) && \
    make install

# 4. Set runtime env
ENV LD_LIBRARY_PATH="/usr/local/lib"

