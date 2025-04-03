#!/bin/bash

APP=$1         # Binary path (e.g., ./client)
TEXTFILE=$2    # Path to input file (e.g., text_files/sample.txt)
OUTFILE=$3     # Output CSV file

/usr/bin/time -v "$APP" "$TEXTFILE" > /dev/null 2> temp_bench.txt

WALL_TIME=$(grep "Elapsed (wall clock)" temp_bench.txt | awk '{print $8}')
MEMORY=$(grep "Maximum resident set size" temp_bench.txt | awk '{print $6}')
USER_TIME=$(grep "User time (seconds)" temp_bench.txt | awk '{print $4}')
SYS_TIME=$(grep "System time (seconds)" temp_bench.txt | awk '{print $4}')
DATE=$(date +%Y-%m-%dT%H:%M:%S)

echo "DEBUG: Time=$WALL_TIME, Mem=$MEMORY, User=$USER_TIME, Sys=$SYS_TIME"

echo "$DATE,$WALL_TIME,$MEMORY,$USER_TIME,$SYS_TIME" >> "$OUTFILE"
rm temp_bench.txt
