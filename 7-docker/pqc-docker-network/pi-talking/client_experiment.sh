#!/bin/bash

read -p "Enter client binary name (e.g., client): " CLIENT_INPUT
read -p "Enter the path to the netsim file (e.g., text_files/sample.txt): " TEXT

CLIENT_FOLDER="client_t"
CLIENT_BIN="./$CLIENT_FOLDER/$CLIENT_INPUT"

if [ ! -x "$CLIENT_BIN" ]; then
    echo "Error: '$CLIENT_BIN' is not executable or not found in ./$CLIENT_FOLDER/"
    exit 1
fi

if [ ! -f "$TEXT" ]; then
    echo "Error: '$TEXT' does not exist."
    exit 1
fi

TEXT_BASENAME=$(basename "$TEXT" .txt)
LOG_FILE="client_${TEXT_BASENAME}_${CLIENT_INPUT}_benchmark.csv"

DURATION=300  # 5 minutes
SLEEP_TIME=5  # To simulate communication

START_TIME=$(date +%s)

echo "[----------${TEXT_BASENAME}]---------- Benchmark started at $(date)"
echo "Logging to $LOG_FILE"

# Optional: add CSV header
if [ ! -f "$LOG_FILE" ]; then
    echo "timestamp,wall_time,mem_kb,user_time,sys_time,idle_mem_kb,temp_c" > "$LOG_FILE"
fi

echo "-------------------------------------------------"

while [ $(($(date +%s) - START_TIME)) -lt $DURATION ]; do
    ./benchmark.sh "$CLIENT_BIN" "$TEXT" "$LOG_FILE"
    sleep $SLEEP_TIME
done

echo "[${TEXT_BASENAME}] Benchmark finished at $(date)"
echo
