# Makefile

CC = gcc
CFLAGS = -L/usr/local/lib
CLIENT_LIBS = -lmbedtls -lmbedx509 -lmbedcrypto -loqs -lssl -lcrypto
SERVER_LIBS = -loqs -lmbedtls -lmbedx509 -lmbedcrypto

CLIENT_SRC = mbedtls_Client.c
CLIENT_OUT = client

SERVER_SRC = mbedtls_Server.c
SERVER_OUT = server

.PHONY: all clean

all: $(CLIENT_OUT) $(SERVER_OUT)

$(CLIENT_OUT): $(CLIENT_SRC)
	$(CC) $(CLIENT_SRC) -o $(CLIENT_OUT) $(CLIENT_LIBS) $(CFLAGS)

$(SERVER_OUT): $(SERVER_SRC)
	$(CC) -o $(SERVER_OUT) $(SERVER_SRC) $(SERVER_LIBS) $(CFLAGS)

clean:
	rm -f $(CLIENT_OUT) $(SERVER_OUT)
