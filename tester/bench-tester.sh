#!/bin/bash

# Run bench_tester.py 40 times and measure CPU and memory usage
for i in {1..400}
do
    echo "Running bench_tester.py - Iteration $i"
    /usr/bin/time -v python3 bike-t.py 2>> bench_usage.log &
    pid=$!
    while kill -0 $pid 2>/dev/null; do
        ps -o pid,%cpu,%mem,vsz,rss,cmd -p $pid >> bench_usage.log
        sleep 1
    done
    wait $pid
    echo "Iteration $i completed"
done

echo "Completed 40 iterations of bench_tester.py"
echo "Check bench_usage.log for CPU and memory usage details."
