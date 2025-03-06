#!/bin/bash

# Paths to the source always connected
SERVER_SRC_alwaysconnected="mbedtls_Server.c"
CLIENT_SRC_alwaysconnected="mbedtls_Client.c"

# Path to source from reconnections
SERVER_SRC_alwaysconnected="mbedtls_Server.c"
CLIENT_SRC_alwaysconnected="mbedtls_Client.c"

# Executable names
SERVER_EXEC_ta="serverAlways"
CLIENT_EXEC_ta="clientAlways"

SERVER_EXEC_tr="serverReconnections"
CLIENT_EXEC_tr="clientReconnections"

# Ports and log files
LOG_FILE_always="server_log.txt"
LOG_FILE_reconnections="server_log_reconnections.txt"
PORT=8080

# 5-minute duration for the client connections
TEST_DURATION=$((1 * 60))  # 5 minutes in seconds
INTERVAL=10  # Time between client connections in seconds

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

# Run the client repeatedly for 5 minutes
echo "[INFO] Running clients for 5 minutes..."
start_time=$(date +%s)
end_time=$((start_time + TEST_DURATION))

while [ $(date +%s) -lt $end_time ]; do
    echo "[INFO] Starting a new client connection..."
    ./$CLIENT_EXEC
    sleep $INTERVAL  # Wait before starting the next client
done

# Check server logs for successful decryption messages
if grep -q "Decrypted message: Hello, PQC-secured World with BIKE-L1!" $LOG_FILE; then
    echo "[SUCCESS] Messages were successfully decrypted by the server!"
else
    echo "[FAILURE] Message decryption failed. Check server logs for more information."
fi

# Clean up
kill $SERVER_PID > /dev/null 2>&1
rm -f $SERVER_EXEC $CLIENT_EXEC
