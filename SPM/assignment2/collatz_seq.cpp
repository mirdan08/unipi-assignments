#include<iostream>
#include<utility>
#include<vector>
#include<string>
#include<algorithm>
#include<chrono>

#include "utilities.hpp"

/*
Sequential implementation of collatz.
*/

int main(int argc,char* argv[]){
    if(argc < 2){
        std::cout << "Usage: "<< argv[0] << " range-1 [range-2] ... [range-n]" << std::endl;
        return 0;
    }

    std::vector<std::pair<int,int>> ranges;

    std::transform( argv +1,argv+argc, std::back_inserter(ranges),parseRange);

    std::cout << ranges.front().first  << " " << ranges.back().second << std::endl;

    auto timeStart= std::chrono::high_resolution_clock::now();
    
    for(const auto& [start,end]:ranges){
        unsigned long long maxSteps=0;
        for(int n=start;n<=end;n++){
            maxSteps=std::max( maxSteps, collatz_steps(n));
        }
        std::cout << start << "-" << end << ": " << maxSteps << std::endl;
    }
    auto timeEnd= std::chrono::high_resolution_clock::now();
    std::cout << "execution time(s): " << (std::chrono::duration<double> (timeEnd - timeStart)).count() << std::endl;
    return 0;
}
