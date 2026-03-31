#!/bin/bash
# Compiles the workload generator and creates all 16 workload files

mkdir -p workLoads

# Compile the workload generator (run from inside src/)
g++ -std=c++20 -Iinclude workload_generator.cpp -o workload_generator

echo "Generating workload files..."

for WORKLOAD in A B C D; do
    for SIZE in 1000 5000 10000 20000; do
        OUTFILE="workLoads/workLoad_${WORKLOAD}_${SIZE}.json"
        echo "  Creating $OUTFILE"
        ./workload_generator --workload $WORKLOAD --size $SIZE --json --save $OUTFILE
    done
done

echo "Done! All 16 workload files created in workLoads/"
