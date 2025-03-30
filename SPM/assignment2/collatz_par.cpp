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
        std::cout << "Usage: "<< argv[0] << " range-1 [range-2] ... [range-n]" << std::endl;
        return 0;
    }
    std::vector<std::pair<int,int>> ranges;

    std::transform( argv +1,argv+argc, std::back_inserter(ranges),parseRange);

    std::cout << ranges.front().first  << " " << ranges.back().second << std::endl;

    int threadsNumber=8;
    int schedulingStrategy=0;
    int taskSize=32;
    int rangesNumber=ranges.size();


    std::vector<unsigned long long> maxRange(rangesNumber,0);

    std::mutex mutex;

    auto blockCyclicDistribution=[&](int threadIdx)->void{       
        for( int i=0;i<rangesNumber; i++ ){
            const auto& [start,end] = ranges[i];
            int rangeSize=end-start+1;

            int rangeBlocksNumber =  std::floor(rangeSize+taskSize-1)/taskSize;

            int taskOffset= threadIdx;
            unsigned long long partialMax=0;
            for(int t=taskOffset;t<rangeBlocksNumber;t+=threadsNumber){
                int taskStart=start+t*taskSize;
                int taskEnd = std::min(taskStart + taskSize - 1, end);
                
                
                std::cout << "range:"<< i << " thread:" << threadIdx <<" range " << taskStart << " " << taskEnd << std::endl;
                
                unsigned long long currMax=partialMaxCollatzStep(taskStart,taskEnd);
                if (partialMax < currMax){
                    partialMax=currMax;
                    //std::cout << "found upper from thread" << threadIdx  << "[" << taskStart << "," << taskEnd << "]" << std::endl;
                } 

            }
            //std::cout<< partialMax << " from  thread " << threadIdx << " range " << i << std::endl;
            //critical section
            {
                std::lock_guard<std::mutex> lock_guard(mutex);
                if (maxRange[i]< partialMax) maxRange[i]=partialMax;
            }

        }
        
    };
    std::vector<std::thread> threads;
    std::vector<std::future<void>> futures;
    for(int t=0;t<threadsNumber;t++){
        std::packaged_task<void(int threadIdx)> task(blockCyclicDistribution);
        futures.emplace_back(task.get_future());
        std::thread thread(std::move(task),t);
        threads.emplace_back(std::move(thread));
    }

    for(auto& future:futures) future.get();
    for(auto& thread:threads) thread.join();

    
    for(int i=0;i< rangesNumber;i++){
        const auto& [start,end] = ranges[i];
        unsigned long long maxSteps= maxRange[i];
        std::cout << start << "-" << end << ": " << maxSteps << std::endl;
    }
   
    return 0;
}