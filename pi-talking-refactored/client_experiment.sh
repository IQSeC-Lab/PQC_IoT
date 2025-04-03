#!/bin/bash

read -p "Enter client binary name (e.g., client): " CLIENT_INPUT

CLIENT_FOLDER="client_t"

CLIENT_BIN="./$CLIENT_FOLDER/$CLIENT_INPUT"

if [ ! -x "$CLIENT_BIN" ]; then
    echo "Error: '$CLIENT_BIN' is not executable or not found in ./$CLIENT_FOLDER/"
    exit 1
fi

TEXT_FOLDER="text_files"
DURATION=300  # 5 minutes per test
SLEEP_TIME=5 # To simulate communicatio

TEXT_FILES=("$TEXT_FOLDER"/*.txt)



for TEXT in "${TEXT_FILES[@]}"; do
    TEXT_BASENAME=$(basename "$TEXT" .txt)
    LOG_FILE="client_${TEXT_BASENAME}_${CLIENT_INPUT}_benchmark.csv"

    START_TIME=$(date +%s)

    echo "[----------${TEXT_BASENAME}]---------- Benchmark started at $(date)"
    echo "Logging to $LOG_FILE"
    echo "timestamp,run_time_seconds" > "$LOG_FILE"
    echo "-------------------------------------------------"
    while [ $(($(date +%s) - START_TIME)) -lt $DURATION ]; do
        
        ./benchmark.sh "$CLIENT_BIN" "$TEXT" "$LOG_FILE"
        
        sleep $SLEEP_TIME
    done

    echo "[${TEXT_BASENAME}] Benchmark finished at $(date)"
    echo
done
