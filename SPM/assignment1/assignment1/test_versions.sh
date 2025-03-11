#!/bin/bash

# Check if at least one argument is provided
if [ "$#" -lt 3 ]; then
    echo "Usage: $0 (yes/no) <number of trials> <values to test>"
    exit 1
fi

# Check if an argument was passed
if [ "$1" == "yes" ]; then
    print_values="1"
else
    print_values=""
fi

iter_num=$2

# Loop through the remaining arguments
shift 2


for arg in "$@"; do
    echo -e "\tTESTING K=$arg"
    for ((i = 1; i <= $iter_num; i++)); do
        echo -e "\t\tITERATION NO.$i"
        echo -e "testing plain"
        ./softmax_plain $arg $print_values
        echo -e "testing avx"
        ./softmax_avx $arg $print_values
        echo -e "testing vectorized"
        ./softmax_auto $arg $print_values
    done

done 