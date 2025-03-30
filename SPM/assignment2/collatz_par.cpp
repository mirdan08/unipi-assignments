#include<iostream>
#include<utility>
#include<vector>
#include<string>
#include<algorithm>
#include<cmath>
#include<future>
#include<mutex>

#include "utilities.hpp"

int main(int argc,char* argv[]){
    if(argc < 2){
        std::cout << "Usage: " << argv[0] << " [-d] [-n num_threads] [-c chunk_size] range1 [range2 ...]\n"
        << "Options:\n"
        << "\t-d                Enable dynamic scheduling (default is block-cyclic)\n"
        << "\t-n num_threads    Set the number of threads (default is 16)\n"
        << "\t-c chunk_size     Set the block/task size (default is 1)\n"
        << "\trange1, range2,... Specify ranges in the form 'start-end' (e.g. 1-100)\n"
        << "\n"
        << "Example: " << argv[0] << " -d -n 8 -c 32 1-100 200-300\n";
        return 0;
    }
    bool doneParsing=false;
    int i=1;
    bool dynamicScheduling=false;
    
    int threadsNumber=8;
    int taskSize=32;
    
    while(!doneParsing){
        std::string current=argv[i];

        if(current[0]!='-'){
            doneParsing=true;
        }
        else if( current == "-d" ){
            dynamicScheduling=true;
        }
        else if( current == "-n" ){
            threadsNumber=atoi(argv[++i]);
        }
        else if( current == "-c" ){
            taskSize=atoi(argv[++i]);
        }
        i++;
    }
    
    std::vector<std::pair<int,int>> ranges;
    
    std::transform( argv +i-1,argv+argc, std::back_inserter(ranges),parseRange);
    int rangesNumber=ranges.size();
    
    std::cout << ranges.front().first  << " " << ranges.back().second << std::endl;
    
    std::vector<unsigned long long> maxRange(rangesNumber,0);
    
    std::mutex mutex;

    auto blockCyclicDistribution=[&](int threadIdx)->std::vector<unsigned long long>{   
        std::vector<unsigned long long> partialMaxRange(rangesNumber,0);    
        for( int i=0;i<rangesNumber; i++ ){
            const auto& [start,end] = ranges[i];
            int rangeSize=end-start+1;
            int rangeBlocksNumber =  std::floor(rangeSize+taskSize-1)/taskSize;
            unsigned long long partialMax=0;
            for(int t=threadIdx;t<rangeBlocksNumber;t+=threadsNumber){
                int taskStart=start+t*taskSize;
                int taskEnd = std::min(taskStart + taskSize - 1, end);
                partialMaxRange[i]=std::max(partialMaxRange[i],partialMaxCollatzStep(taskStart,taskEnd));
            }
        }

        return partialMaxRange;
        
    };

    std::vector<std::thread> threads;
    std::vector<std::future<std::vector<unsigned long long>>> futures;

    for(int t=0;t<threadsNumber;t++){
        std::packaged_task<std::vector<unsigned long long>(int threadIdx)> task(blockCyclicDistribution);
        futures.emplace_back(task.get_future());
        std::thread thread(std::move(task),t);
        threads.emplace_back(std::move(thread));
    }
    
    for(auto& future:futures) {
        std::vector<unsigned long long> partialMax= future.get();
        for(int i=0;i<rangesNumber;i++){
            maxRange[i]=std::max(maxRange[i],partialMax[i]);
        }
    }
    for(auto& thread:threads) thread.join();

    for(int j=0;j< rangesNumber;j++){
        const auto& [start,end] = ranges[j];
        unsigned long long maxSteps= maxRange[j];
        std::cout << start << "-" << end << ": " << maxSteps << std::endl;
    }
   
    return 0;
}