# Assignment no 1 delivery

This is the delivery for the assignment no.2

For this assignment the task was to test a static and a cyclical distribution to compute the maximum collatz problem steps.

In the include directory you can find:
- inside the `collatz.cpp` file a base collatz implementation and a partial version used to compute the maximum steps for a certain range.
- inside the `taskfactory.hpp` an unsued task factory useful to avoid C++ jargon for types.
- a `utilities.cpp` file containing a function to make parsing ranges from the argument easier.

then we have the `collatz_par.cpp` and `collatz_seq.cpp` files, the first contains the actual implementation of the two scheduling strategies and the other one conains just the sequential version used as a baseline.

furthere useful files are:
- a `benchmark.sh` file containing a bash script to test out the performances and generate files to analyze.
- a `analysis.py` python script used to generate some statistics and metrics useful to analyze performances.

Lastly in the benchmarks folder you can find the rsults obtained from running the remote cluster used to test the algorithm.

You can compile the program by running `make collatz_[par|seq]`. for collat_par you can specify also the scheduling type to use, thu number of threads and the hcunk size, see the usage for more information by running the programs without specifying any argument.