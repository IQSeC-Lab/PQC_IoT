#!/bin/bash

# Run bench_tester.py 40 times
for i in {1..40}
do
    echo "Running bench_tester.py - Iteration $i"
    python3 bench_tester.py
done

echo "Completed 40 iterations of bench_tester.py"
