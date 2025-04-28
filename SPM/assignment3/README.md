# Assignment no 3 delivery

This is the delivery for the assignment no.3

For this asignment the task was using OMP to parallelize compression and decompression of files using miniz.

The original sequential version is in `minizseq.cpp`, while the main ,task based,implementation is present in `minizpar_omptask.cpp` but a second version done without tasks is present in the file `minizpar_omp.cpp` as well and was used for the first tests. Bot the version test results are in the `results` directory.

Additionally from the original specification a `-B` flag was added to specify block size, the default value when not specified is 1000 bytes for each block.

to analyze the results you can use the following scripts:
- `analyze.py` which given a csv format of the iterations averages them and gives out also the variance
- `speedup.py` which takesthe result from the previous script and calculates speedup then for each combination of thread and scheduling picks the one with better speedup and also plots a graph of the speedups.

to crete the benchmarks you can use the following scripts:
- `generates_files.sh` which given a directory name and a number of repetitions creates file in the sizes of 128,256,512 MBs and 1,2,5,10 KBs.
- `generate_performance_benchmarks.sh` does the same thing but in two different directories which was used for the benchmarks, one for lot of small files benchmark and one for few big files benchmark.

to run the benchmarks you can use the `performance_benchmarks.sh` script which will generate the performance benchmarks and run a a search with various parameters on the task based version of the project.

The project can be compile with the command `make [minizseq|minizpar_omp|minizpar_omptask]` depending on which version you want to use, the script to run benchmarks has been modified to run only on the last version but you can easily change it to run on the previous one.