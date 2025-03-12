#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <limits>      
#include <hpc_helpers.hpp>
#include <avx_mathfun.h>
#include <immintrin.h>

void softmax_avx(const float *input, float *output, size_t K) {


	size_t vect_K= std::floor(K/8)*8;
	size_t rest_K=K-vect_K;
	bool is_not8mul=rest_K>0;
	size_t i;

	__m256 acc_max256=_mm256_loadu_ps(&input[0]);
	//iterate on data using AVX registers
	for(i=8;i<vect_K;i+=8){
		__m256 curr256= _mm256_loadu_ps(&input[i]);
		acc_max256 = _mm256_max_ps(acc_max256,curr256);
	}
	//load last remaining values when K not a multiply of 8
	if(is_not8mul){
		float max=_mm256_cvtss_f32(acc_max256);
		for(i=0;i<rest_K;++i){
			max=std::max(max,output[vect_K+i]);
		}
		__m256 rest_max256=_mm256_set_ps(max,0,0,0,0,0,0,0);
		acc_max256=_mm256_max_ps(acc_max256,rest_max256);
	}
	//extract actual max for partial maximums

	//start comparing first 4 with last 4 elements
	//extract halfs
	__m128 low128 = _mm256_extractf128_ps(acc_max256,0);
	__m128 high128 = _mm256_extractf128_ps(acc_max256, 1);
	
	//compare halfs
	__m128 max128 = _mm_max_ps(low128,high128);
	
	//swap to compare first with second half
	__m128 shuffle128 = _mm_permute_ps(max128,_MM_SHUFFLE(2,3,0,1));
	max128 = _mm_max_ps(max128,shuffle128);
	//now you have pairwise max just swap couples and repeat
	shuffle128 = _mm_permute_ps(max128,_MM_SHUFFLE(1,0,3,2));
	max128 = _mm_max_ps(max128,shuffle128);
	//all elements of max128 are the highest possible value now, just copy into the 256 one
	acc_max256 = _mm256_insertf128_ps(acc_max256, max128, 0);
	acc_max256 = _mm256_insertf128_ps(acc_max256, max128, 1);
	
	//use the AVX to substract and exponentiate 
	//also accumulate exponential sums in the process
	__m256 exp_sum256=_mm256_set1_ps(0.0f);
	for(i=0;i<vect_K;i+=8){
		__m256 curr_input256=_mm256_loadu_ps(&input[i]);
		__m256 curr_output256=_mm256_loadu_ps(&output[i]);
		curr_output256=exp256_ps(
			_mm256_sub_ps(
				curr_input256,
				acc_max256
			)
		);
		exp_sum256 = _mm256_add_ps(exp_sum256,curr_output256);	
		_mm256_storeu_ps(&output[i],curr_output256);
	} 
	float max=_mm256_cvtss_f32(acc_max256);
	float exp_sum_values[8];
	
	_mm256_storeu_ps(exp_sum_values,exp_sum256);

	//handle non multiples of 8 values
	for(i=0;i<rest_K;++i){
		output[vect_K+i]=std::exp(input[vect_K+i]-max);
		exp_sum_values[i]+=output[vect_K+i];
	}
	//calculate final summation
	float sum_div=0;
	for(i=0;i<8;++i){
		sum_div+=exp_sum_values[i];
	}
	exp_sum256=_mm256_set1_ps(sum_div);
	//summation is done and values of exp are stored in output, divide by summation using AVX
	for(i=0;i<vect_K;i+=8){
		__m256 curr_output256=_mm256_loadu_ps(&output[i]);
		curr_output256=_mm256_div_ps(curr_output256,exp_sum256);
		_mm256_storeu_ps(&output[i],curr_output256);
	}
	//handle remainders 
	if (is_not8mul){
		float exp_sum=_mm256_cvtss_f32(exp_sum256);
		for(i=0;i<rest_K;++i){
			output[vect_K+i]/=exp_sum;
		}
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

