#!/bin/bash

APP=$1         # Binary path (e.g., ./client)
TEXTFILE=$2    # Path to input file (e.g., text_files/sample.txt)
OUTFILE=$3     # Output CSV file

# Capture system idle memory before execution
IDLE_MEMORY=$(free | grep Mem | awk '{print $3}')
TEMP=$(vcgencmd measure_temp | grep -oP '[0-9]+\.[0-9]+')

/usr/bin/time -v "$APP" "$TEXTFILE" > /dev/null 2> temp_bench.txt

WALL_TIME=$(grep "Elapsed (wall clock)" temp_bench.txt | awk '{print $8}')
MEMORY=$(grep "Maximum resident set size" temp_bench.txt | awk '{print $6}')
USER_TIME=$(grep "User time (seconds)" temp_bench.txt | awk '{print $4}')
SYS_TIME=$(grep "System time (seconds)" temp_bench.txt | awk '{print $4}')
DATE=$(date +%Y-%m-%dT%H:%M:%S)

echo "DEBUG: Time=$WALL_TIME, Mem=$MEMORY, User=$USER_TIME, Sys=$SYS_TIME, Temp=$TEMP°C"

# Optional: Add header if file doesn't exist
if [ ! -f "$OUTFILE" ]; then
    echo "Timestamp,WallTime,MaxRSS,UserTime,SysTime,IdleMemory,TempC" >> "$OUTFILE"
fi

echo "$DATE,$WALL_TIME,$MEMORY,$USER_TIME,$SYS_TIME,$IDLE_MEMORY,$TEMP" >> "$OUTFILE"
rm temp_bench.txt
