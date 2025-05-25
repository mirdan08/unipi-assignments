#!/bin/bash
#SBATCH --job-name=mpi_benchmark_ass4
#SBATCH --output=benchmark_output_%j.log
#SBATCH --time=00:30:00
#SBATCH --nodes=8
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=32
#SBATCH --exclusive

leaf_size=(100 10000 100000)
num_threads=(1 2 4 8 16 32)
node_size=(2 3 4 5 6 7 8)
#array_size=(1000000 10000000)
array_size=(1000000 10000000)
payload_size=(100)
trials=5

make cleanall
make mergeSortPar
make sortSeq
make mergeSortDist

echo "iteration,leaf_size,num_threads,payload_size,array_size,mpi_nodes,ff_par_time,sort_time,dist_time"

for i in $(seq 1 $trials); do
    for nn in "${node_size[@]}"; do
        for as in "${array_size[@]}"; do
            for ps in "${payload_size[@]}"; do
                for n in "${num_threads[@]}"; do
                    sort_output="$(./sortSeq -s $as -r $ps)"
                    sort_time=$(echo "$sort_output" | tail -n 1 | cut -d':' -f2)

                    for ls in "${leaf_size[@]}"; do
                        # Run distributed MPI job with $nn nodes
                        dist_par_output=$(mpirun -N=$nn ./mergeSortDist -s $as -r $ps -l $ls -t $n -v 0)
                        dist_par_time=$(echo "$dist_par_output" | tail -n 1 | cut -d':' -f2)

                        ff_par_output=$(./mergeSortPar -s $as -r $ps -l $ls -t $n -v 0)
                        ff_par_time=$(echo "$ff_par_output" | tail -n 1 | cut -d':' -f2)

                        echo "$i,$ls,$n,$ps,$as,$nn,$ff_par_time,$sort_time,$dist_par_time"
                    done
                done
            done
        done
    done
done
