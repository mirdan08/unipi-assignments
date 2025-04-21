#include <config.hpp>
#include <cmdline.hpp>
#include <utility.hpp>
#include <vector>
#include<cmath>
#include<iostream>

/*
In this implementation we need a header for the compression part as
we have to memorize at least the singly compressed blocks
*/

static inline bool compressChunkData(
	unsigned char *startPtr, 
	size_t size,
	//output params
	size_t& chunk_len,
	unsigned char **chunkPtr
) {
	
	unsigned char * inPtr  = startPtr;
	size_t          inSize = size;
	// get an estimation of the maximum compression size
	size_t cmp_len = compressBound(inSize);
	// allocate memory to store compressed data in memory
	unsigned char *ptrOut = new unsigned char[cmp_len];
	if (compress(ptrOut, &cmp_len, (const unsigned char *)inPtr, inSize) != Z_OK) {
		return false;
	}
	//save output params
	chunk_len=cmp_len;
	*chunkPtr=ptrOut;
	return true;
}

static inline bool decompressChunkData(
	unsigned char *chunkStartPtr, 
	size_t chunk_size,
	//output params
	unsigned char **decompChunkPtr,
	size_t& decompChunkLen
){
    size_t decompressedSize=chunk_size;  // read the original size

    // Write the decompressed data to a file
	unsigned char *decompressed_data=new unsigned char[chunk_size];
	// decompress the data 	
    if (uncompress(decompressed_data, &decompressedSize, chunkStartPtr, chunk_size) != Z_OK) {
        return false;
    }
	decompChunkLen=decompressedSize;
	*decompChunkPtr=decompressed_data;
	return true;

}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage(argv[0]);
        return -1;
    }
    long start=parseCommandLine(argc, argv);

    bool success=true;

    size_t blockSize=1000;

	
	while(argv[start]) {
		size_t filesize=0;
		if (isDirectory(argv[start], filesize)) {
			success &= walkDir(argv[start],COMP);
		} else {
			//load file by mapping it to the memory
			unsigned char *ptr = nullptr;
			if (!mapFile(argv[start], filesize, ptr)) {
				if (QUITE_MODE>=1) 
				std::fprintf(stderr, "mapFile %s failed\n", argv[start]);
				return false;
			}
			
			if(COMP){
				size_t chunksNumber=(filesize + blockSize - 1) / blockSize;

				std::string fname=std::string(argv[start])+ ".pzip";
				std::ofstream outFile(fname,std::ios::binary);
				//prepare dynamic length header based on file size and block size
				std::vector<size_t> chunksLengths(chunksNumber);
				std::vector<unsigned char*> chunksData(chunksNumber);

				
				for(int i=0;i<chunksNumber;i++){
					unsigned char* ptrStart=ptr+i*blockSize;
					
					size_t blockOffset = i * blockSize;
					size_t chunksSize = std::min(blockSize, filesize - blockOffset);
					
					size_t compChunkLen=-1;
					unsigned char* compressedChunk=nullptr;
					
					success&=compressChunkData(
						ptrStart,chunksSize,
						compChunkLen,&compressedChunk
					);
					
					chunksLengths[i]=compChunkLen;
					chunksData[i]=compressedChunk;
            	}
				//write number of chunks
				outFile.write(reinterpret_cast<const char*>(&chunksNumber),sizeof(size_t));
				//write chunks lengths
				for(size_t i=0;i<chunksNumber;i++){
					outFile.write(reinterpret_cast<const char*>(&chunksLengths[i]), sizeof(size_t));

				}

				// write actual chunkData
				for(int i=0;i<chunksNumber;i++){
					outFile.write(reinterpret_cast<const char*>(chunksData[i]), sizeof(unsigned char)*chunksLengths[i]);
				}
				outFile.close();
				
			}
			else{
				
				size_t chunksNumber;
				std::memcpy(&chunksNumber,ptr,sizeof(size_t));
				size_t offset=sizeof(size_t);
				std::vector<size_t> chunksLenghts(chunksNumber);
				std::vector<size_t> decompChunksLenghts(chunksNumber);
				std::vector<size_t> chunksIdx(chunksNumber);
				std::vector<unsigned char*> chunksData(chunksNumber);
				
				for(int i=0;i<chunksNumber;i++){
					std::memcpy(&chunksLenghts[i],ptr+offset,sizeof(size_t));
					offset+=sizeof(size_t);
				}

				int accumIdx=0;

				for(int i=0;i<chunksNumber;i++){
					chunksIdx[i]=offset+accumIdx;
					accumIdx+=chunksLenghts[i];
					std::cout << chunksLenghts[i] << std::endl; 
				}

				for(int i=0;i<chunksNumber;i++){
					unsigned char* decompChunkData;
					size_t decompChunkLen;
					success&=decompressChunkData(
						&ptr[chunksIdx[i]],chunksLenghts[i],
						
						&chunksData[i],decompChunksLenghts[i]
					);
				}
				std::ofstream outFile(argv[start],std::ios::binary);

				for(int i=0;i< chunksNumber;i++){
					outFile.write(reinterpret_cast<const char*>(chunksData[i]),decompChunksLenghts[i]);
				}
				outFile.close();
			}

			unmapFile(ptr, filesize);
            

		}
		start++;
	}
	if (!success) {
		printf("Exiting with (some) Error(s)\n");
		return -1;
	}
	printf("Exiting with Success\n");
	return 0;
}