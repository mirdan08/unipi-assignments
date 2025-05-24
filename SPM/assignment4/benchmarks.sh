#!/bin/bash

leaf_size=(100 10000 100000)
num_threads=(1 2 4 8 16 32)
#array_size=(1000000 10000000 100000000 1000000000)

#node_size=(2 3 4 5 6 7 8)
node_size=(3)
array_size=(1000000 10000000 )
payload_size=(100 )
trials=5

make cleanall

make mergeSortPar
make sortSeqmake
make mergeSortDist

echo "iteration,leaf_size,num_threads,payload_size,array_size,mpi_nodes,ff_par_time,sort_time,dist_time"
for i in $(seq 1 $trials); do
    for n in "${num_threads[@]}"; do
        for as in "${array_size[@]}"; do
            for ps in "${payload_size[@]}"; do
                for nn in "${node_size[@]}"; do
                    #seq_output="$(./mergeSortSeq -s $as -r $ps)"
                    #last_line=$(echo -e "$seq_output" | tail -n 1)
                    #seq_time=$(echo $last_line | cut -d':' -f2)

                    sort_output="$(./sortSeq -s $as -r $ps)"
                    last_line=$(echo -e "$sort_output" | tail -n 1)
                    sort_time=$(echo $last_line | cut -d':' -f2)

                    dist_par_output="$(mpirun -N $nn mergeSortDist -s $as -r $ps -l $ls -t $n -v 0)"
                    
                    echo $dist_par_output
                    
                    last_line=$(echo -e "$dist_par_output" | tail -n 1)
                    dist_par_time=$(echo $last_line | cut -d':' -f2)

                    for ls in "${leaf_size[@]}"; do
                        ff_par_output="$(./mergeSortPar -s $as -r $ps -l $ls -t $n -v 0)"
                        last_line=$(echo -e "$ff_par_output" | tail -n 1)
                        ff_par_time=$(echo $last_line | cut -d':' -f2)
                        

                        echo "$i,$ls,$n,$ps,$as,$nn,$ff_par_time,$sort_time,$dist_par_time"
                    done
                done
            done
        done
    done
done