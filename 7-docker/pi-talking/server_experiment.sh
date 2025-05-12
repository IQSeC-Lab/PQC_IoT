#!/bin/bash

read -p "Enter server binary name (e.g., server): " SERVER_INPUT
read -p "Enter the path to the netsim file (e.g., text_files/sample.txt): " TEXT

SERVER_FOLDER="server_t"
SERVER_BIN="./$SERVER_FOLDER/$SERVER_INPUT"

if [ ! -x "$SERVER_BIN" ]; then
    echo "Error: '$SERVER_BIN' is not executable or not found in ./$SERVER_FOLDER/"
    exit 1
fi

if [ ! -f "$TEXT" ]; then
    echo "Error: '$TEXT' does not exist."
    exit 1
fi

TEXT_BASENAME=$(basename "$TEXT" .txt)
LOG_FILE="server_${TEXT_BASENAME}_${SERVER_INPUT}_benchmark.csv"

DURATION=300  # 5 minutes
SLEEP_TIME=5

echo "[----------${TEXT_BASENAME}]---------- Server benchmark started at $(date)"
echo "Logging to $LOG_FILE"

# Add CSV header if not already present
if [ ! -f "$LOG_FILE" ]; then
    echo "timestamp,wall_time,mem_kb,user_time,sys_time,idle_mem_kb,temp_c" > "$LOG_FILE"
fi

# Start the server binary in background (without wrapping in benchmark.sh)
"$SERVER_BIN" "$TEXT" &
SERVER_PID=$!

START_TIME=$(date +%s)

# Logging loop — every 5 seconds while server runs
while [ $(($(date +%s) - START_TIME)) -lt $DURATION ]; do
    ./benchmark.sh /proc/$SERVER_PID/exe "$TEXT" "$LOG_FILE"
    sleep $SLEEP_TIME
done

# Kill server
kill "$SERVER_PID" 2>/dev/null
wait "$SERVER_PID" 2>/dev/null

echo "[${TEXT_BASENAME}] Server benchmark finished at $(date)"
echo
