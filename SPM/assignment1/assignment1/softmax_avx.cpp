#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <limits>      
#include <hpc_helpers.hpp>
#include <avx_mathfun.h>
#include <immintrin.h>
#define NUM_REGS 8


//reminder a float is 32bit/4bytes here
//we assume 256 bits for now
void softmax_avx(const float *input, float *output, size_t K) {

	size_t vect_K= std::floor(K/NUM_REGS)*NUM_REGS;
	size_t rest_K=K-vect_K;
	
	__m256 acc_max256=_mm256_loadu_ps(&input[0]);
	//iterate on aligned data
	for(size_t i=8;i<vect_K;i+=8){
		__m256 curr256= _mm256_loadu_ps(&input[i]);
		acc_max256 = _mm256_max_ps(acc_max256,curr256);
	}
	//gather remaining unaligned data
	//__m256 remaining256=_mm256_loadu_ps(
	//	&input[K-8]
	//);
	//acc_max256 = _mm256_max_ps(acc_max256,remaining256);
	
	//start comparing first 4 with last 4 elements

	//extract halfs
	__m128 low128 = _mm256_castps256_ps128(acc_max256);
	__m128 high128 = _mm256_extractf128_ps(acc_max256, 1);
	
	//compare halfs
	__m128 max128 = _mm_max_ps(low128,high128);
	//swap to compare first with second half
	__m128 shuffle128 = _mm_shuffle_ps(max128,max128,_MM_SHUFFLE(2,3,0,1));
	max128 = _mm_max_ps(max128,shuffle128);
	//now you have pairwise max just swap couples and repeat
	shuffle128 = _mm_shuffle_ps(max128,max128,_MM_SHUFFLE(1,0,3,2));
	max128 = _mm_max_ps(max128,shuffle128);
	//all elements of max128 are the highest possible value now, just copy into the 256 one
	acc_max256 = _mm256_insertf128_ps(acc_max256, max128, 0);
	acc_max256 = _mm256_insertf128_ps(acc_max256, max128, 1);
	
	//use the 256bit to substract and exponentiate efficiently
	__m256 exp_sum256=_mm256_set_ps(0,0,0,0,0,0,0,0);
	for(size_t i=0;i<vect_K;i+=8){
		__m256 curr256=_mm256_loadu_ps(&input[i]);
		__m256 curr_output256=_mm256_loadu_ps(&output[i]);
		curr_output256=exp256_ps(
			_mm256_sub_ps(
				curr256,
				acc_max256
			)
		);
		exp_sum256 = _mm256_add_ps(exp_sum256,curr_output256);	
		_mm256_storeu_ps(&output[i],curr_output256);
	}

	//summation is done and values of exp are stored, divide by summation
	for(size_t i=8;i<vect_K;i+=8){
		__m256 curr_output256=_mm256_loadu_ps(&output[i]);
		curr_output256=_mm256_div_ps(curr_output256,exp_sum256);
		_mm256_storeu_ps(&output[i],curr_output256);
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

	TIMERSTART(softime_avx);
	softmax_avx(input.data(), output.data(), K);
	TIMERSTOP(softime_avx);
	
	// print the results on the standard output
	if (print) {
		printResult(output, K);
	}
}

