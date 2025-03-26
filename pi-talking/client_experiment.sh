#!/bin/bash

DURATION=60  # 5 minutes in seconds
START_TIME=$(date +%s)

echo "Client benchmarking started at $(date)"
echo "Running for $DURATION seconds..."

while [ $(($(date +%s) - START_TIME)) -lt $DURATION ]; do
    echo "[RUN $(date '+%H:%M:%S')] Connecting to server..."
    ./benchmark.sh ./client client_benchmark.csv
    sleep 5  # optional: pause between runs
done

echo "Client finished at $(date)"
