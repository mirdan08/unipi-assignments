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
node_size=(1 2 3 4 5 6 7 8)
array_size=(1000000 10000000 1000000000 1000000000)
payload_size=(100 1000 10000)
trials=5


echo "iteration,leaf_size,num_threads,payload_size,array_size,mpi_nodes,ff_par_time,sort_time,dist_time"

for i in $(seq 1 $trials); do
    for ps in "${payload_size[@]}"; do
        make cleanall >/dev/null 2>&1
        make mergeSortPar RPAYLOAD=$ps >/dev/null 2>&1
        make sortSeq RPAYLOAD=$ps >/dev/null 2>&1
        make mergeSortDist RPAYLOAD=$ps >/dev/null 2>&1
        for nn in "${node_size[@]}"; do
            for as in "${array_size[@]}"; do
                for n in "${num_threads[@]}"; do
                    sort_output="$(srun --nodes=1 \
                            --ntasks-per-node=1 \
                            --time=00:2:00 \
                            --mpi=pmix  \
                            ./mergeSortDist -s $as -r $ps)"
                    sort_time=$(echo "$sort_output" | tail -n 1 | cut -d':' -f2)

                    for ls in "${leaf_size[@]}"; do
                       dist_par_output="$(srun --nodes=$nn \
                            --ntasks-per-node=1 \
                            --time=00:2:00 \
                            --mpi=pmix  \
                            ./mergeSortDist -s $as -r $ps -l $ls -t $n -v 0)"

                        dist_par_time=$(echo "$dist_par_output" | tail -n 1 | cut -d':' -f2)


                        ff_par_output="$(srun --nodes=1 \
                            --ntasks-per-node=1 \
                            --time=00:2:00 \
                            --mpi=pmix  \
                            ./mergeSortDist -s $as -r $ps -l $ls -t $n -v 0)"
                        ff_par_time=$(echo "$ff_par_output" | tail -n 1 | cut -d':' -f2)

                        echo "$i,$ls,$n,$ps,$as,$nn,$ff_par_time,$sort_time,$dist_par_time"
                    done
                done
            done
        done
    done
done
