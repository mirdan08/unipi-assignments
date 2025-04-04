#!/bin/bash

make cleanall

make collatz_par
make collatz_seq

c_values=(1000 2500 5000 10000)
n_values=(1 2 4 8 16 24 32)

n_repetitions=$1

# Ensure n_repetitions is provided
if [[ -z "$n_repetitions" ]]; then
    echo "Usage: $0 <number_of_repetitions>"
    exit 1
fi

# Define ranges if necessary
ranges="1-1000  50000000-100000000  1000000000-1100000000"  

echo "BENCHMARK RESULTS"
echo "chunk size,num threads,iteration,dynamic,static block cyclic,sequential"
for ((i=1; i<=n_repetitions; i++)); do
    sequentialResult=$(./collatz_seq $ranges | tail -n 1 | awk -F': ' '{print $2}')
    for c in "${c_values[@]}"; do   
        for n in "${n_values[@]}"; do
            dynamicParResult=$(./collatz_par -d -c $c -n $n $ranges| tail -n 1 | awk -F': ' '{print $2}')
            staticParResult=$(./collatz_par -c $c -n $n $ranges | tail -n 1 | awk -F': ' '{print $2}')
            echo "$c,$n,$i,$dynamicParResult,$staticParResult,$sequentialResult"
        done
    done
done
