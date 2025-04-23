#!/bin/bash

threads_num=(1 2 4 8 16 32)
omp_schedule=("dynamic" "static" "guided")
block_sizes=(1000 10000 100000 1000000)


dir_path="$1"
reps="$2"

echo "now testing performances on $dir_path"
echo "schedule_type,threads_number,block_sizes(bytes),iteration,compile_time(s),decompile_time(s)"

for ((i = 1; i <= reps; i++)); do
    seq_perf_comp=$(./minizseq -C 1 "$dir_path" | grep -oP 'execution time\(s\):\s*\K[0-9.]+')
    seq_perf_decomp=$(./minizseq -D 1 "$dir_path" | grep -oP 'execution time\(s\):\s*\K[0-9.]+')
    echo "sequential,0,0,$i,$seq_perf_comp,$seq_perf_decomp"

    for n_threads in "${threads_num[@]}"; do
        for schedule_type in "${omp_schedule[@]}"; do
            for block_size in "${block_sizes[@]}"; do
                export OMP_NUM_THREADS=$n_threads
                export OMP_SCHEDULE="$schedule_type,$n_threads"

                par_perf_comp=$(./minizpar -C 1 -B "$block_size" "$dir_path" | grep -oP 'execution time\(s\):\s*\K[0-9.]+')
                par_perf_decomp=$(./minizpar -D 1 "$dir_path" | grep -oP 'execution time\(s\):\s*\K[0-9.]+')

                echo "$schedule_type,$n_threads,$block_size,$i,$par_perf_comp,$par_perf_decomp"
            done
        done
    done
done
