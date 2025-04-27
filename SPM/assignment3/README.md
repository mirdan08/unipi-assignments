# Assignment no 3 delivery

This is the delivery for the assignment no.3

For this asignment the task was using OMP to parallelize compression and decompression of files using miniz.

The original file is in minizseq.cpp, while the main implementation is present in minizpar_omptask.cpp but second version done without tasks is present in the file as well and was used for the first tests.

additionally from the original specification a -B flag was added to specify block size, the default value when not specified is 1000 bytes.

to analyze the results you can use the following scripts:
- analyze.py which given a csv format of the iterations averages them and gives out also the variance
- speedup.py which takesthe result from the previous script and calculates speedup then for each combination of thread and scheduling picks the one with better speedup.

to crete the benchmarks you can use the following scripts:
- generates_files.sh which given a directory name and a number of repetitions creates file in the sizes of 128,256,512 MBs and 1,2,5,10 KBs.
- generate_performance_benchmarks.sh does the thing but in two different directories which was used for the benchmarks.

to run the benchmarks you can use the performance_benchmarks.sh script which will gneerat the performance benchmarks and run a a search with various parameters on the task based version of the project.