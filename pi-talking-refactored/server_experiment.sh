#!/bin/bash

read -p "Enter server binary name (e.g., server): " SERVER_INPUT

SERVER_FOLDER="server_t"
SERVER_BIN="./$SERVER_FOLDER/$SERVER_INPUT"

if [ ! -x "$SERVER_BIN" ]; then
    echo "Error: '$SERVER_BIN' is not executable or not found in ./$SERVER_FOLDER/"
    exit 1
fi

TEXT_FOLDER="text_files"
DURATION=300  # 5 minutes


TEXT_FILES=("$TEXT_FOLDER"/*.txt)

for TEXT in "${TEXT_FILES[@]}"; do
    TEXT_BASENAME=$(basename "$TEXT" .txt)
    LOG_FILE="server_${TEXT_BASENAME}_${SERVER_INPUT}_benchmark.csv"

    echo "[----------${TEXT_BASENAME}]---------- Server benchmark started at $(date)"
    echo "Logging to $LOG_FILE"
    echo "timestamp,wall_time,mem_kb,user_time,sys_time,idle_mem_kb,temp_c" > "$LOG_FILE"
    echo "-------------------------------------------------"

    # Start server in background
    ./benchmark.sh "$SERVER_BIN" "$TEXT" "$LOG_FILE" &
    SERVER_PID=$!

    # Let it run for the duration
    sleep $DURATION

    # Kill the server
    kill "$SERVER_PID" 2>/dev/null
    wait "$SERVER_PID" 2>/dev/null

    echo "[${TEXT_BASENAME}] Server benchmark finished at $(date)"
    echo
done
