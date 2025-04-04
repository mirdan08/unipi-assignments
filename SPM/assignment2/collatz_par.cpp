#include<iostream>
#include<utility>
#include<vector>
#include<deque>
#include<string>
#include<algorithm>
#include<cmath>
#include<future>
#include<mutex>
#include<ctime>
#include<chrono>

#include "utilities.hpp"
#include "taskfactory.hpp"

int main(int argc,char* argv[]){
    if(argc < 2){
        std::cout << "Usage: " << argv[0] << " [-d] [-n num_threads] [-c chunk_size] range1 [range2 ...]\n"
                << "Options:\n"
                << "\t-d                Enable dynamic scheduling (default is block-cyclic)\n"
                << "\t-n num_threads    Set the number of threads (default is 16)\n"
                << "\t-c chunk_size     Set the block/task size (default is 32)\n"
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
    
    //arguments parsing
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
    
    auto start= std::chrono::high_resolution_clock::now();
    //ranges parsing
    std::vector<std::pair<int,int>> ranges;
    
    std::transform( argv +i-1,argv+argc, std::back_inserter(ranges),parseRange);
    int rangesNumber=ranges.size();
    
    std::cout << ranges.front().first  << " " << ranges.back().second << std::endl;
    
    //array containing the final computation of each range
    std::vector<unsigned long long> maxRange(rangesNumber,0);
    
    std::function<std::vector<unsigned long long>(int)> blockCyclicDistribution=[&](int threadIdx)->std::vector<unsigned long long>{   
        std::vector<unsigned long long> partialMaxRange(rangesNumber,0);    
        for( int i=0;i<rangesNumber; i++ ){
            const auto& [start,end] = ranges[i];
            int rangeSize=end-start+1;
            int rangeBlocksNumber =  std::floor(rangeSize+taskSize-1)/taskSize;
            
            
            for(int t=threadIdx;t<rangeBlocksNumber;t+=threadsNumber){
                int taskStart=start+t*taskSize;
                int taskEnd = std::min(taskStart + taskSize - 1, end);
                partialMaxRange[i]=std::max(partialMaxRange[i],partialMaxCollatzStep(taskStart,taskEnd));
            }
        }
        
        return partialMaxRange;
        
    };
    
    //dynamic scheduling shared variables
    std::mutex chunksMutex;

    int bufferSize=std::max(threadsNumber+1,(int)ranges.size()+1);
    
    //fixed size circular buffer
    int bufferReadIdx=0;
    int bufferWriteIdx=0;

    std::vector <std::pair<int,int>> availableChunks(bufferSize,{-1,-1});
    
    for(int i=0;i<(int)ranges.size();i++){
        //initializare with the starting chunk of each range
        availableChunks[i].first=i;
        availableChunks[i].second=0;
    }
    
    bufferWriteIdx=ranges.size();
    int remainingRanges= ranges.size();
    std::function<std::vector<unsigned long long>(int)> dynamicDistribution=[&](int threadIdx)->std::vector<unsigned long long>{   
        
        std::vector<unsigned long long> partialMaxRange(rangesNumber,0);   
        
        int currTaskIdx=-1;
        int currTaskChunk=-1;
        
        unsigned long long chunkStart=-1;
        unsigned long long chunkEnd=-1;
        bool terminate=false;
        
        while(true){
            {
                std::lock_guard<std::mutex> lock_guard(chunksMutex);
                //we check if there is work to do
                if(remainingRanges==0){
                    terminate=true;
                    break;
                }
                
                //we are sure that there is work to do
                const auto& [taskIdx,taskChunk]=availableChunks[bufferReadIdx];
                bufferReadIdx=(bufferReadIdx+1)%bufferSize;
                if((taskChunk+1)*taskSize+1 < ranges[taskIdx].second){
                    // next chunk is available -> add it to queue
                    availableChunks[bufferWriteIdx].first=taskIdx;
                    availableChunks[bufferWriteIdx].second=taskChunk+1;
                    
                    bufferWriteIdx=(bufferWriteIdx+1)%bufferSize;
                }
                else{
                    // next chunk is not available -> this range is terminated
                    remainingRanges--;
                }
                
                //save the extracted task data
                currTaskIdx=taskIdx;
                currTaskChunk=taskChunk;
            }  
            //if work is done exit
            if (terminate == true){
                break;
            }
            
            //process the element
            
            chunkStart= ranges[currTaskIdx].first+currTaskChunk*taskSize+1;
            chunkEnd=std::min(
                ranges[currTaskIdx].first+(currTaskChunk+1)*taskSize,
                ranges[currTaskIdx].second
            );
            
            partialMaxRange[currTaskIdx]=std::max(
                partialMaxRange[currTaskIdx],
                partialMaxCollatzStep(chunkStart,chunkEnd)
            );
        }
        
        
        return partialMaxRange;
        
    };
    
    std::vector<std::thread> threads;
    std::vector<std::future<std::vector<unsigned long long>>> futures;
    std::vector<std::packaged_task<std::vector<unsigned long long>(int)>> tasks;

    tasks.reserve(threadsNumber);
    for(int t=0;t<threadsNumber;t++){
        std::function<std::vector<unsigned long long>(int)> schedulingStrategy = dynamicScheduling? dynamicDistribution:blockCyclicDistribution;
        tasks.emplace_back(schedulingStrategy);
        std::packaged_task<std::vector<unsigned long long>(int)> task(schedulingStrategy);
        futures.emplace_back(task.get_future());
        threads.emplace_back(std::move(task),t);
    }
    for(auto& thread:threads) thread.join();

    for(auto& future:futures) {
        std::vector<unsigned long long> partialMax= future.get();
        for(int i=0;i<rangesNumber;i++){
            maxRange[i]=std::max(maxRange[i],partialMax[i]);
        }
    }
    
    
    
    for(int j=0;j< rangesNumber;j++){
        const auto& [start,end] = ranges[j];
        unsigned long long maxSteps= maxRange[j];
        std::cout << start << "-" << end << ": " << maxSteps << std::endl;
    }

    auto end= std::chrono::high_resolution_clock::now();

    std::cout << "execution time(s): " << (std::chrono::duration<double> (end - start)).count() << std::endl;
    
    return 0;
}