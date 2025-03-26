#!/bin/bash
./server &
SERVER_PID=$!
sleep 60
kill $SERVER_PID
echo "Server done"
