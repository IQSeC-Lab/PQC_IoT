#!/bin/bash

# Paths to the source files
SERVER_SRC="mbedtls_Server.c"
CLIENT_SRC="mbedtls_Client.c"

# Executable names
SERVER_EXEC="server"
CLIENT_EXEC="client"

# Ports and log files
LOG_FILE="server_log.txt"
PORT=8080

# 5-minute duration for the client
CLIENT_RUNTIME=$((5 * 60))  # 5 minutes in seconds

# Clean up previous builds and logs
rm -f $SERVER_EXEC $CLIENT_EXEC $LOG_FILE

# Compile the server
echo "[INFO] Compiling server..."
gcc $SERVER_SRC -o $SERVER_EXEC -loqs -lmbedtls -lmbedcrypto -lmbedx509
if [ $? -ne 0 ]; then
    echo "[ERROR] Failed to compile server."
    exit 1
fi

# Compile the client
echo "[INFO] Compiling client..."
gcc $CLIENT_SRC -o $CLIENT_EXEC -loqs -lmbedtls -lmbedcrypto -lmbedx509
if [ $? -ne 0 ]; then
    echo "[ERROR] Failed to compile client."
    exit 1
fi

# Start the server in the background and redirect output to log file
echo "[INFO] Starting server..."
./$SERVER_EXEC > $LOG_FILE 2>&1 &
SERVER_PID=$!

# Wait for the server to start
sleep 2

# Run the client for 5 minutes
echo "[INFO] Running client for 5 minutes..."
timeout $CLIENT_RUNTIME ./$CLIENT_EXEC

# Give some time for the final communication
sleep 2

# Check if server received and decrypted messages correctly
if grep -q "Decrypted message: Hello, PQC-secured World with BIKE-L1!" $LOG_FILE; then
    echo "[SUCCESS] Message was successfully decrypted by the server!"
else
    echo "[FAILURE] Message decryption failed. Check server logs for more information."
fi

# Clean up
kill $SERVER_PID > /dev/null 2>&1
rm -f $SERVER_EXEC $CLIENT_EXEC
