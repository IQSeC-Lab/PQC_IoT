#!/bin/bash

# Run server in background for 1 minute
./run_server.sh &

# Wait a bit to ensure server is ready
sleep 3

# Run client multiple times while server is alive
for i in {1..10}; do
    echo "[$i] Running client benchmark..."
    ./benchmark.sh ./client client_benchmark.csv
    sleep 5  # Wait between runs
done
