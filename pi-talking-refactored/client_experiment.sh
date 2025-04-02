#!/bin/bash

# CLIENT_FOLDER="client_t"
CLIENT_BIN="./client"
TEXT_FOLDER="text_files"
DURATION=300  # 5 minutes per test
SLEEP_TIME=5

KEM_ALGOS=(
    OQS_KEM_alg_bike_l1
    OQS_KEM_alg_bike_l3
    OQS_KEM_alg_bike_l5
    OQS_KEM_alg_kyber_512
    OQS_KEM_alg_kyber_768
    OQS_KEM_alg_kyber_1024
    OQS_KEM_alg_hqc_128
    OQS_KEM_alg_hqc_192
    OQS_KEM_alg_hqc_256
)

TEXT_FILES=("$TEXT_FOLDER"/*.txt)

if [ ! -x "$CLIENT_BIN" ]; then
    echo "Error: '$CLIENT_BIN' is not executable."
    exit 1
fi

for KEM in "${KEM_ALGOS[@]}"; do
    for TEXT in "${TEXT_FILES[@]}"; do
        TEXT_BASENAME=$(basename "$TEXT" .txt)
        LOG_FILE="client_${KEM}_${TEXT_BASENAME}_benchmark.csv"

        START_TIME=$(date +%s)

        echo "[$KEM + $TEXT_BASENAME] Benchmark started at $(date)"
        echo "Logging to $LOG_FILE"
        echo "timestamp,run_time_seconds" > "$LOG_FILE"

        while [ $(($(date +%s) - START_TIME)) -lt $DURATION ]; do
            echo "[RUN $(date '+%H:%M:%S')] Running: $CLIENT_BIN $KEM $TEXT"

            RUN_START=$(date +%s.%N)
            ./benchmark.sh "$CLIENT_BIN" "$KEM" "$TEXT"
            RUN_END=$(date +%s.%N)

            ELAPSED=$(echo "$RUN_END - $RUN_START" | bc)
            TIMESTAMP=$(date '+%Y-%m-%d %H:%M:%S')

            echo "$TIMESTAMP,$ELAPSED" >> "$LOG_FILE"
            sleep $SLEEP_TIME
        done

        echo "[$KEM + $TEXT_BASENAME] Benchmark finished at $(date)"
        echo
    done
done
