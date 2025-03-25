#!/bin/bash

# Start server in background
./server &

# Store PID
SERVER_PID=$!

# Run for 60 seconds
sleep 60

# Kill server after timeout
kill $SERVER_PID

echo "Server ran for 60 seconds and was terminated."
