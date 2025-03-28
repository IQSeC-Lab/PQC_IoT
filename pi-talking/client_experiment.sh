#!/bin/bash

# Prompt for client binary name (without ./)
read -p "🟢Enter client binary name (e.g., client): " CLIENT_INPUT

# Auto-prepend ./ to the name
CLIENT_BIN="./$CLIENT_INPUT"

# Check if it exists and is executable
if [ ! -x "$CLIENT_BIN" ]; then
    echo " Error: '$CLIENT_BIN' is not executable or not found."
    exit 1
fi

# Prepare CSV log filename
LOG_FILE="${CLIENT_INPUT}_benchmark.csv"
DURATION=300  # in seconds
START_TIME=$(date +%s)

echo "Client benchmarking started at $(date)"
echo "Logging results to $LOG_FILE"
echo "timestamp,run_time_seconds" > "$LOG_FILE"

while [ $(($(date +%s) - START_TIME)) -lt $DURATION ]; do
    echo "[RUN $(date '+%H:%M:%S')] Running: $CLIENT_BIN"

    RUN_START=$(date +%s.%N)
    ./benchmark.sh "$CLIENT_BIN" "$LOG_FILE"
    RUN_END=$(date +%s.%N)

    ELAPSED=$(echo "$RUN_END - $RUN_START" | bc)
    TIMESTAMP=$(date '+%Y-%m-%d %H:%M:%S')

    echo "$TIMESTAMP,$ELAPSED" >> "$LOG_FILE"
    sleep 5
done

echo "✅ Client finished at $(date)"
