# Compiler and flags
CC = gcc
CFLAGS = -I/home/ubuntu/Desktop/liboqs/include \
         -I/home/ubuntu/path/to/mbedtls/include \
		 -I/home/ubuntu/path/to/OpenSSL/include \
LDFLAGS = -L/home/ubuntu/Desktop/liboqs/build \
          -L/home/ubuntu/path/to/mbedtls/library \
		  -L/home/ubuntu/path/to/OpenSSL/library \
          -loqs -lcrypto -lssl -lmbedtls -lmbedx509 -lmbedcrypto -lm \
          -Wl,-rpath,/home/ubuntu/Desktop/liboqs/build

# Source files for each executable
SRCS_SERVER = mbedtls_Server.c
SRCS_CLIENT = mbedtls_Client.c 

# Executable targets
TARGETS = server client

# Object files for each executable
OBJS_SERVER = $(SRCS_SERVER:.c=.o)
OBJS_CLIENT = $(SRCS_CLIENT:.c=.o)

# Build all targets
all: $(TARGETS) clean_objs  # Add a step to clean .o files after build

# Server executable
server: $(OBJS_SERVER)
	$(CC) $(OBJS_SERVER) -o server $(LDFLAGS)

# Client executable
client: $(OBJS_CLIENT)
	$(CC) $(OBJS_CLIENT) -o client $(LDFLAGS)

# Compile each .c file to .o file
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Remove .o files after build
clean_objs:
	rm -f $(OBJS_SERVER) $(OBJS_CLIENT)

# Clean all build files
clean:
	rm -f $(OBJS_SERVER) $(OBJS_CLIENT) $(TARGETS)
