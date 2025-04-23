#!/bin/bash


reps="$1"

benchmark_dir="$2"

mkdir -p "$(benchmark_dir)"

for ((i=1; i <= reps; i++)); do
    dd if=/dev/urandom of=${benchmark_dir}/bigfile128M${i}.dat bs=2M count=64 
    dd if=/dev/urandom of=${benchmark_dir}/bigfile256M${i}.dat bs=2M count=128 
    dd if=/dev/urandom of=${benchmark_dir}/bigfile512M${i}.dat bs=2M count=256 
    dd if=/dev/urandom of=${benchmark_dir}/bigfile1K${i}.dat bs=1K count=1
    dd if=/dev/urandom of=${benchmark_dir}/bigfile2K${i}.dat bs=1K count=2 
    dd if=/dev/urandom of=${benchmark_dir}/bigfile5K${i}.dat bs=1K count=5
    dd if=/dev/urandom of=${benchmark_dir}/bigfile10K${i}.dat bs=1K count=10
done

echo "benchmark created"