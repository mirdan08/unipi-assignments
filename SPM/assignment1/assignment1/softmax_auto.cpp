#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <limits>      
#include <hpc_helpers.hpp>

void softmax_auto(const float* __restrict__ input, float* __restrict__ output, const size_t K) {
	
	float max_val = -std::numeric_limits<float>::infinity();
//	#pragma GCC unroll 4
	for (size_t i = 0; i < K; ++i) {
		max_val = std::max(max_val, input[i]);
	}

	#pragma GCC ivdep
	for(size_t i=0;i<K;++i){
		output[i]=std::exp(input[i]-max_val);
	}
	
	float sum=0;
	#pragma GCC ivdep
	for(size_t i=0;i<K;++i){
		sum+=output[i];
	}
	#pragma GCC ivdep
	for(size_t i=0;i<K;++i){
		output[i]/=sum;
	}
}


std::vector<float> generate_random_input(size_t K, float min = -1.0f, float max = 1.0f) {
    std::vector<float> input(K);
    //std::random_device rd;
    //std::mt19937 gen(rd());
	std::mt19937 gen(5489); // fixed seed for reproducible results
    std::uniform_real_distribution<float> dis(min, max);
    for (size_t i = 0; i < K; ++i) {
        input[i] = dis(gen);
    }
    return input;
}

void printResult(std::vector<float> &v, size_t K) {
	for(size_t i=0; i<K; ++i) {
		std::fprintf(stderr, "%f\n",v[i]);
	}
}


int main(int argc, char *argv[]) {
	if (argc == 1) {
		std::printf("use: %s K [1]\n", argv[0]);
		return 0;		
	}
	size_t K=0;
	if (argc >= 2) {
		K = std::stol(argv[1]);
	}
	bool print=false;
	if (argc == 3) {
		print=true;
	}	
	std::vector<float> input=generate_random_input(K);
	std::vector<float> output(K);

	TIMERSTART(softime_auto);
	softmax_auto(input.data(), output.data(), K);
	TIMERSTOP(softime_auto);
	
	// print the results on the standard output
	if (print) {
		printResult(output, K);
	}
}
