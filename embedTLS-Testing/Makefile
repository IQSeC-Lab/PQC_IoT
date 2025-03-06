# Compiler and flags
CC = gcc
CFLAGS = -I/home/ubuntu/Desktop/liboqs/include \
         -I/home/ubuntu/path/to/mbedtls/include
		 
LDFLAGS = -L/home/ubuntu/Desktop/liboqs/build \
          -L/home/ubuntu/path/to/mbedtls/library \
          -loqs -lmbedtls -lmbedx509 -lmbedcrypto -lm \
          -Wl,-rpath,/home/ubuntu/Desktop/liboqs/build

# Source files
SRCS = mbedtls_Server.c mbedtls_Client.c

# Object files
OBJS = mbedtls_Server.o mbedtls_Client.o

# Build all object files
all: $(OBJS)

# Compile each .c file to .o file
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean all build files
clean:
	rm -f $(OBJS)
