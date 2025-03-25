#!/bin/bash

# Define server and client executable names
SERVER_EXEC="server"
CLIENT_EXEC="client"

# Define log and results files
RESULTS="results.txt"
BENCH_LOG="bench_usage.log"
PORT=8080

# Test duration and interval for client connections
TEST_DURATION=$((1 * 60))  # 5 minutes in seconds
INTERVAL=5  # Time between client connections in seconds

# Clean up previous logs
rm -f $RESULTS $BENCH_LOG

# Start the server in the background
echo "[INFO] Starting server..."
./$SERVER_EXEC &
SERVER_PID=$!

# Wait for the server to start
sleep 2

# Run the client repeatedly while measuring performance
echo "[INFO] Running clients for $TEST_DURATION seconds..."
start_time=$(date +%s)
end_time=$((start_time + TEST_DURATION))

while [ $(date +%s) -lt $end_time ]; do
    echo "[INFO] Starting a new client connection..."
    
    # Run the client and measure its CPU & memory usage
    /usr/bin/time -v ./$CLIENT_EXEC 2>> $BENCH_LOG &
    
    pid=$!
    while kill -0 $pid 2>/dev/null; do
        ps -o pid,%cpu,%mem,vsz,rss,cmd -p $pid >> $BENCH_LOG
        sleep 1
    done
    wait $pid

    echo "[INFO] Client iteration completed."
    sleep $INTERVAL
done

# Stop the server
echo "[INFO] Stopping the server..."
kill $SERVER_PID > /dev/null 2>&1

echo "[INFO] Test completed. Check $BENCH_LOG for performance results."
