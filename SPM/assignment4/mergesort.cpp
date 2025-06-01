//parallelized FastFlow version
#include<vector>
#include <ff/ff.hpp>
#include "mergesort.hpp"
#include<iostream>
#include<random>

class StreamParser: public ff::ff_node_t<OrderedArray,PosKeyVec>
{
    public: 
        StreamParser(OrderedArray& datastream,size_t leafSize)
            :datastream(datastream),
            leafSize(leafSize){}
        PosKeyVec* svc(OrderedArray* in){
            
            int n = datastream.size();
            for(int i=0;i<n;i+=leafSize){
                //start from base case and order if necessary
                size_t offset= (n-i>=leafSize)?leafSize: n-i;
                PosKeyVec* partialRes=new PosKeyVec();
                partialRes->reserve(offset);  // reserve to avoid reallocations

                for (size_t i = 0; i < offset; ++i) {
                    partialRes->emplace_back(i,datastream[i].key );
                }
                //send into the pipeline
                ff_send_out(partialRes);
            }
            return EOS;
        };
    private:
        OrderedArray& datastream;
        size_t leafSize;
};

struct MasterNode: public ff::ff_minode_t<PosKeyVec,Task_t>
{
    MasterNode(size_t stop_size,PosKeyVec** result_ptr)
        :stop_size(stop_size),
         result_ptr(result_ptr){}
    public:
        Task_t* svc(PosKeyVec* in){
            //you received the last ordered array, stop
            if(in->size()==stop_size){
                *result_ptr=in;
                return EOS;
            }
            //buffer is empty add an element to it
            if(bufferedValue==nullptr){
                bufferedValue=in;
                return GO_ON;
            }
            Task_t* res=new Task_t(in,bufferedValue);
            ff_send_out(res);
            bufferedValue=nullptr;

            return GO_ON;

        }
    private:
        PosKeyVec* bufferedValue=nullptr;
        PosKeyVec** result_ptr=nullptr;
        size_t stop_size=0;
};

inline void merge_sort_pairs(
    const PosKeyPair* __restrict__ A,
    size_t sizeA,
    const PosKeyPair* __restrict__ B,
    size_t sizeB,
    PosKeyPair* __restrict__ Out)
{
    size_t i = 0, j = 0, k = 0;

    // Merge both arrays until one is exhausted
    while (i < sizeA && j < sizeB) {
        // Inline comparison — this is usually cheaper than a ternary
        const int ak = A[i].second;
        const int bk = B[j].second;

        const bool takeA = ak <= bk;
        Out[k++] = takeA ? A[i++] : B[j++];
    }

    // Copy any remaining elements from A
    if (i < sizeA) {
        const size_t remaining = sizeA - i;
        std::memcpy(Out + k, A + i, remaining * sizeof(PosKeyPair));
        k += remaining;
    }

    // Copy any remaining elements from B
    if (j < sizeB) {
        const size_t remaining = sizeB - j;
        std::memcpy(Out + k, B + j, remaining * sizeof(PosKeyPair));
        k += remaining;
    }
}

struct Worker: public ff::ff_node_t<Task_t,PosKeyVec>
{
    public:
        PosKeyVec* svc(Task_t* in){
            PosKeyVec* res;
            PosKeyVec* arr1=in->first;
            PosKeyVec* arr2=in->second;
            res=new PosKeyVec(arr1->size()+arr2->size());
            merge_sort_pairs(arr1->data(),arr1->size(),arr2->data(),arr2->size(),res->data());
            ff_send_out(res);
            delete arr1;
            delete arr2;
            return GO_ON;
        }

};

struct SortingWorker: public ff::ff_monode_t<PosKeyVec,PosKeyVec>{
    PosKeyVec* svc(PosKeyVec* in){
        std::sort(
                    in->begin(),in->end(),
                    [](const PosKeyPair& a,const PosKeyPair& b){return a.second<b.second;}
                );
        return in;
    }
};

std::unique_ptr<ff::ff_pipeline> spawn_ff_merging_pipeline(
    std::vector<Record>& src,
    PosKeyVec** dst,
    size_t numWorkers,
    size_t numSorters,
    size_t leafSize
) {
    auto pipeline = std::make_unique<ff::ff_pipeline>();

    auto sp = std::make_unique<StreamParser>(src, leafSize);
    pipeline->add_stage(sp.release()); // release the raw pointer to FastFlow

    if (numSorters >= 1) {
        auto sorting_farm = std::make_unique<ff::ff_farm>();
        std::vector<ff::ff_node*> sorting_workers;
        for (int i = 0; i < numSorters; ++i) {
            sorting_workers.push_back(new SortingWorker());
        }
        sorting_farm->add_workers(sorting_workers);
        sorting_farm->remove_collector();

        pipeline->add_stage(sorting_farm.release()); // pass ownership
    }

    auto merging_farm = std::make_unique<ff::ff_farm>();
    auto master = new MasterNode(src.size(), dst);
    merging_farm->add_emitter(master);

    std::vector<ff::ff_node*> farm_workers;
    for (int i = 0; i < numWorkers; ++i) {
        farm_workers.push_back(new Worker());
    }

    merging_farm->add_workers(farm_workers);
    merging_farm->remove_collector();
    merging_farm->set_scheduling_ondemand();
    merging_farm->wrap_around();
    pipeline->add_stage(merging_farm.release()); // pass ownership

    return pipeline;
}

std::vector<Record> spawn_datastream(size_t arraySize,size_t recordSize,unsigned int seed){
    std::mt19937 gen(seed); 

    std::uniform_int_distribution<> keyDis(0, 10000000);
    std::uniform_int_distribution<>  charDis(33, 126);


    std::vector<Record> dataStream(arraySize);
    std::transform(
        dataStream.begin(), dataStream.end(), 

        dataStream.begin(),
                [&](Record){ 
                    Record r;
                    r.key=keyDis(gen);
                    //r.rpayload=(char*)malloc(sizeof(char)*recordSize);
                    for(int i=0;i<RPAYLOAD;i++){
                        r.rpayload[i]=charDis(gen);
                    }
                    return r;
                }
        );
    return dataStream;
}
