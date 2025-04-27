#!/bin/bash

echo "now generating benchmarks for various purposes"
bigfile_reps=1
echo "making few big files with $bigfile_reps repetitions"

mkdir -p bigfiles_dir
mkdir -p smallfiles_dir

for ((i=1; i <= bigfile_reps; i++)); do
    dd if=/dev/urandom of=bigfiles_dir/bigfile50M${i}.dat bs=2M count=25
    dd if=/dev/urandom of=bigfiles_dir/bigfile100M${i}.dat bs=2M count=50 
    dd if=/dev/urandom of=bigfiles_dir/bigfile200M${i}.dat bs=2M count=100 

done


smallfile_reps=500
echo "making lots of small files behaviour with $smallfile_reps"

for ((i=1; i <= smallfile_reps; i++)); do
    dd if=/dev/urandom of=smallfiles_dir/bigfile5K${i}.dat bs=1K count=5
    dd if=/dev/urandom of=smallfiles_dir/bigfile10K${i}.dat bs=1K count=10
done

echo "benchmarks done"