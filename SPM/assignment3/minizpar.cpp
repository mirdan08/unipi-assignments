#include <config.hpp>
#include <cmdline.hpp>
#include <utility.hpp>
#include <vector>
#include<cmath>


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
	chunkPtr=&ptrOut;
	return true;
}

static inline bool decompressChunkData(
	unsigned char *chunkStartPtr, 
	size_t chunk_size,
	//output params
	unsigned char **decompChunkPtr,
	size_t& decompChunkLen
){
    size_t decompressedSize= chunk_size;  // read the original size
	chunkStartPtr += sizeof(size_t); // advance the pointer

    // Write the decompressed data to a file
	unsigned char *decompressed_data=nullptr;
	// decompress the data 	
    if (uncompress(decompressed_data, &decompressedSize, chunkStartPtr, chunk_size) != Z_OK) {
        return false;
    }
	decompChunkLen=decompressedSize;
	decompChunkPtr=&decompressed_data;
	return true;

}

struct chunk_t{
	size_t chunk_size;
	unsigned char* chunkData;
};

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
			std::string fname=std::string(argv[start])+ ".zip";
			std::ofstream outFile(fname,std::ios::binary);
			size_t chunksNumber=std::ceil(filesize/blockSize);
			//prepare dynamic length header based on file size and block size
			std::vector<size_t> chunksLengths(chunksNumber);
			std::vector<unsigned char*> chunkData(chunksNumber);
			chunksLengths.push_back(-1);

			//off set for the actual data
			size_t dataOffset=chunksLengths.size();
            for(int i=0;i<chunksNumber;i++){
				unsigned char* ptrStart=(unsigned char*)(ptr+i);
				
                size_t chunksSize = i*blockSize+ blockSize < filesize ? blockSize : filesize - i*blockSize ;
				
				size_t compChunkLen=-1;
				unsigned char* compressedChunk=nullptr;
				
				success&=compressChunkData(
					ptrStart,chunksSize,
					compChunkLen,&compressedChunk
				);
				
				chunksLengths[i]=compChunkLen;
				chunkData[i]=compressedChunk;
            }
			int accumIndex=0;
			for(int i=0;i<chunksNumber;i++){
				//write chun length
				outFile.write(reinterpret_cast<const char*>(&chunksLengths[i]), sizeof(chunksLengths[i]));	
				//write chunk index
				outFile.write(reinterpret_cast<const char*>(&dataOffset+accumIndex), sizeof(dataOffset+accumIndex));
				//write actual copressed data
				outFile.seekp(dataOffset+accumIndex);
				outFile.write(reinterpret_cast<const char*>(&chunkData[i]), sizeof(chunkData[i]));
				accumIndex+=chunksLengths[i];


			}
			// at this point the files are ready and we can serialize the header
			outFile.write(reinterpret_cast<const char*>(&chunksLengths), sizeof(chunksLengths));	
			//serialize the file sizes
			for(int blockIdx=0;blockIdx<filesize;blockIdx+=blockSize){
				
				
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