#!/bin/bash

leaf_size=(100 10000 100000)
num_threads=(1 2 4 8 16 32)
node_size=(1 2 3 4 5 6 7 8)
array_size=(1000000 10000000 1000000000 1000000000)
payload_size=(100 1000 10000)
trials=5


echo "iteration,leaf_size,num_threads,payload_size,array_size,mpi_nodes,ff_par_time,sort_time,dist_time"

for i in $(seq 1 $trials); do
    for ps in "${payload_size[@]}"; do
        make cleanall >/dev/null 2>&1
        make mergeSortPar RPAYLOAD="$ps" >/dev/null 2>&1
        make sortSeq RPAYLOAD="$ps" >/dev/null 2>&1
        make mergeSortDist RPAYLOAD="$ps" >/dev/null 2>&1
        for nn in "${node_size[@]}"; do
            for as in "${array_size[@]}"; do
                for n in "${num_threads[@]}"; do
                    # Run srun for sortSeq and filter out srun messages
                    sort_output=$(srun --time=00:5:00 ./sortSeq -s "$as" -r "$ps" 2>&1 | tail -n 1 | cut -d':' -f2 )
                    sort_time=$(echo "$sort_output" | tail -n 1 | cut -d':' -f2)

                    for ls in "${leaf_size[@]}"; do
                        # Run distributed parallel merge sort
                        dist_par_output=$(srun --mpi=pmix --cpu-bind=none -N $nn -n $nn  --time=00:5:00 --mpi=pmix ./mergeSortDist -s "$as" -r "$ps" -l "$ls" -t "$n" -v 0 2>&1 | tail -n 1 | cut -d':' -f2)
                        dist_par_time=$(echo "$dist_par_output" | tail -n 1 | cut -d':' -f2)

                        # Run fork-join parallel merge sort
                        ff_par_output=$(srun --time=00:5:00 ./mergeSortPar -s "$as" -r "$ps" -l "$ls" -t "$n" -v 0 2>&1 | tail -n 1 | cut -d':' -f2)
                        ff_par_time=$(echo "$ff_par_output" | tail -n 1 | cut -d':' -f2)

                        line="$i,$ls,$n,$ps,$as,$nn,$ff_par_time,$sort_time,$dist_par_time"
                        echo "$line"
                        results+="$line"$'\n'
                    done
                done
            done
        done
    done
done

echo "$line"