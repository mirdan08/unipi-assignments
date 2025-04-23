#!/bin/bash

make cleanall

make minizseq

make minizpar

bash generate_performance_benchmarks.sh

bash benchmark.sh bigfiles_dir 5
bash benchmark.sh smallfiles_dir 5