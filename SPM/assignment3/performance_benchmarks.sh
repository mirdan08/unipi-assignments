#!/bin/bash

rm -r bigfiles_dir smallfiles_dir

make cleanall
rm minizseq minizpar_omptask

make minizseq

make minizpar_omptask

bash generate_performance_benchmarks.sh

bash benchmark.sh bigfiles_dir 5
bash benchmark.sh smallfiles_dir 5

rm -r bigfiles_dir smallfiles_dir