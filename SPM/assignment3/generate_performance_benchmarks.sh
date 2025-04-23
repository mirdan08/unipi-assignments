#!/bin/bash

echo "now generating benchmarks for various purposes"
bigfile_reps=10
echo "making few big files with $bigfile_reps repetitions"

mkdir -p bigfiles_dir
mkdir -p smallfiles_dir

for ((i=1; i <= bigfile_reps; i++)); do
    dd if=/dev/urandom of=bigfiles_dir/bigfile128M${i}.dat bs=2M count=64 
    dd if=/dev/urandom of=bigfiles_dir/bigfile256M${i}.dat bs=2M count=128 
    dd if=/dev/urandom of=bigfiles_dir/bigfile512M${i}.dat bs=2M count=256 
done


smallfile_reps=1000
echo "making lots of small files behaviour with $smallfile_reps"

for ((i=1; i <= smallfile_reps; i++)); do
    dd if=/dev/urandom of=smallfiles_dir/bigfile1K${i}.dat bs=1K count=1
    dd if=/dev/urandom of=smallfiles_dir/bigfile2K${i}.dat bs=1K count=2 
    dd if=/dev/urandom of=smallfiles_dir/bigfile5K${i}.dat bs=1K count=5
    dd if=/dev/urandom of=smallfiles_dir/bigfile10K${i}.dat bs=1K count=10
done

echo "benchmarks done"