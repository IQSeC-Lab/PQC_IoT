#!/bin/bash

# Ask the user for the server binary name
read -p "Enter server binary name (e.g., server): " SERVER_INPUT

# Folder where server binaries are stored
SERVER_FOLDER="server_t"

# Path to the server binary
SERVER_BIN="./$SERVER_FOLDER/$SERVER_INPUT"

# Check if it exists and is executable
if [ ! -x "$SERVER_BIN" ]; then
    echo "Error: '$SERVER_BIN' not found or not executable."
    exit 1
fi

# Run the server in the background
echo "Starting server: $SERVER_BIN"
$SERVER_BIN &
SERVER_PID=$!


sleep 1260

# Kill the server
kill $SERVER_PID
echo "Server done (PID $SERVER_PID)"
