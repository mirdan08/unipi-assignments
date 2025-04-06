# Assignment no 1 delivery

This is the delivery for the assignment no.1

For this assignment i had to use AVX intrinsics and autovecorization options to implement an optmized version of the softmax function you can find the `softmax_plain.cpp` file. The implementation are respectively in the `softmax_avx.cpp` and `softmax_auto.cpp` files with the Makefile to compile them.

The codes can be compiled with the command `make softmax_[plain|avx|auto]`.

To compare the different version you can also use `bash test_value.sh K [1]` to compile and test the softmax implementations for a certain number of values, specify an argument you want after the script if you also want to see the output values. 

If you wan you can also repeat the test for a set of values with repetitions use `bash test_versions.sh (yes/no) <number of repetitions> Kvalue_1 .... Kvalue_N`, the yes/no value is to specify if you also want to print the values being calculated.

For the delivery i also run two experiments to test performances:
- one to test various scales with powers of 2 which can be run with `bash test_power2.sh`.
- one to test high values at dfferent scales with non multiples of 8 can be run with`bash test_adv.sh`.

This implementation was tested using the above scripts with slurm, you can see my benchmarks inside the `cluster_results` folder.