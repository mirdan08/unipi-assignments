#!/bin/bash
make cleanall

make softmax_plain
make softmax_auto
make softmax_avx

# Run your commands and capture the outputs
echo "testing plain"
./softmax_plain $1 $2
echo "testing avx"
./softmax_avx $1 $2
echo "testing vectorized"
./softmax_auto $1 $2
echo "finished testing"