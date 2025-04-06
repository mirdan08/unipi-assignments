#!/bin/bash

make cleanall

make softmax_auto
make softmax_plain
make softmax_avx

echo "testing plain"
./softmax_plain $1 $2
echo "testing avx"
./softmax_avx $1 $2
echo "testing vectorized"
./softmax_auto $1 $2
echo "finished testing"