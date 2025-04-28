#include <config.hpp>
#include <cmdline.hpp>
#include <utility.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <cstdio>
#include <filesystem>
#include <chrono>

//compress a single chunk data
static inline bool compressChunkData(
	unsigned char *startPtr,
	size_t size,
	size_t &chunk_len,
	unsigned char **chunkPtr)
{

	unsigned char *inPtr = startPtr;
	size_t inSize = size;
	size_t cmp_len = compressBound(inSize);
	unsigned char *ptrOut = new unsigned char[cmp_len];
	if (compress(ptrOut, &cmp_len, (const unsigned char *)inPtr, inSize) != Z_OK)
	{
		return false;
	}
	chunk_len = cmp_len;
	*chunkPtr = ptrOut;
	return true;
}
//decompress a single chunk of data
static inline bool decompressChunkData(
	unsigned char *chunkStartPtr,
	size_t chunk_size,
	unsigned char **decompChunkPtr,
	size_t &decompChunkLen)
{
	size_t decompressedSize = chunk_size;
	unsigned char *decompressed_data = new unsigned char[chunk_size];
	if (uncompress(decompressed_data, &decompressedSize, chunkStartPtr, chunk_size) != Z_OK)
	{
		return false;
	}
	decompChunkLen = decompressedSize;
	*decompChunkPtr = decompressed_data;
	return true;
}
//remove the suffix from the file name
std::string removeSuffix(const std::string &str, const std::string &suffix)
{
	if (str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0)
	{
		return str.substr(0, str.size() - suffix.size());
	}
	return str;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		usage(argv[0]);
		return -1;
	}
	long start = parseCommandLine(argc, argv);
	size_t blockSize = BLOCK_SIZE;
	bool success = true;
	std::vector<std::string> paths;
	int fIdx=start;
	//read files paths on case of folders
	while(argv[start]) 
	{
		if (std::filesystem::exists(argv[fIdx]))
		{
			if (std::filesystem::is_directory(argv[fIdx]))
			{
				if(RECUR){
					for (const auto &entry : std::filesystem::recursive_directory_iterator(argv[fIdx]))
					{
						if(entry.is_regular_file()){
							paths.push_back(entry.path());
						}
					}
				}else{
					for (const auto &entry : std::filesystem::directory_iterator(argv[fIdx]))
					{
						if(entry.is_regular_file()){
							paths.push_back(entry.path());
						}
					}
				}
			}
			else if (std::filesystem::is_regular_file(argv[fIdx]))
			{
				paths.push_back(std::string(argv[fIdx]));
			}
		}
		else
		{
			std::cout << "Path does not exist: " << argv[fIdx] << std::endl;
		}
		start++;
	}
	//main loop iteration
	auto startTime= std::chrono::high_resolution_clock::now();
	for (int pIdx = 0; pIdx < paths.size(); pIdx++)
	{
		size_t filesize = 0;
		
		// load file by mapping it to the memory
		unsigned char *ptr = nullptr;
		if (!mapFile(paths[pIdx].c_str(), filesize, ptr))
		{
			if (QUITE_MODE >= 1)
			std::fprintf(stderr, "mapFile %s failed\n", paths[pIdx]);
			success = false;
			break;
		}
		if (COMP)
		{
			size_t chunksNumber = (filesize + blockSize - 1) / blockSize;

			std::vector<size_t> chunksLenghts(chunksNumber);
			std::vector<unsigned char *> chunksData(chunksNumber);
			//parallelize on blocks of each file
			#pragma omp parallel shared(blockSize) \
			shared(argv)                       \
			shared(pIdx)                       \
			shared(success)                    \
			shared(ptr)                        \
			shared(chunksLenghts)              \
			shared(chunksData)
			
			{
				#pragma omp for reduction(&& : success) schedule(runtime)
				for (int i = 0; i < chunksNumber; i++)
				{
					unsigned char *ptrStart = ptr + i * blockSize;
					
					size_t blockOffset = i * blockSize;
					size_t chunksSize = std::min(blockSize, filesize - blockOffset);
					
					size_t compChunkLen = -1;
					unsigned char *compressedChunk = nullptr;
					
					success &= compressChunkData(
						ptrStart, chunksSize,
						compChunkLen, &compressedChunk);
						
						chunksLenghts[i] = compChunkLen;
						chunksData[i] = compressedChunk;
				}
					//write back the the blocks plus the header
				#pragma omp single
				{
					std::string fname = std::string(paths[pIdx]) + ".pzip";
					std::ofstream outFile(fname, std::ios::binary);
					outFile.write(reinterpret_cast<const char *>(&chunksNumber), sizeof(size_t));
					for (size_t i = 0; i < chunksNumber; i++)
					{
						outFile.write(reinterpret_cast<const char *>(&chunksLenghts[i]), sizeof(size_t));
					}
					for (size_t i = 0; i < chunksNumber; i++)
					{
						outFile.write(reinterpret_cast<const char *>(chunksData[i]), sizeof(unsigned char) * chunksLenghts[i]);
						delete[] chunksData[i];
					}
					outFile.close();
				}
			}
		}
		else//decompression part
		{
			//read back the the header and prepare necessary data for deocompression
			size_t chunksNumber;
			std::memcpy(&chunksNumber, ptr, sizeof(size_t));
			size_t offset = sizeof(size_t);
			std::vector<size_t> decompChunksLenghts(chunksNumber);
			std::vector<size_t> chunksIdx(chunksNumber);
			std::vector<size_t> chunksLenghts(chunksNumber);
			std::vector<unsigned char *> chunksData(chunksNumber);
			//offset is used to allow parallelism
			for (int i = 0; i < chunksNumber; i++)
			{
				std::memcpy(&chunksLenghts[i], ptr + offset, sizeof(size_t));
				offset += sizeof(size_t);
			}
			
			int accumIdx = 0;
			for (int i = 0; i < chunksNumber; i++)
			{
				chunksIdx[i] = offset + accumIdx;
				accumIdx += chunksLenghts[i];
			}
			//main parallelism on the file chunks
			#pragma omp parallel shared(ptr, chunksIdx, chunksLenghts, chunksData, decompChunksLenghts, chunksNumber)
			{
				#pragma omp for reduction(&& : success) schedule(runtime)
				for (int i = 0; i < chunksNumber; i++)
				{
					success &= decompressChunkData(
						&ptr[chunksIdx[i]], chunksLenghts[i],
						&chunksData[i], decompChunksLenghts[i]);
				}
					
				#pragma omp single
				{
					//write back the result
					std::ofstream outFile(removeSuffix(std::string(paths[pIdx]), ".pzip"), std::ios::binary);
					for (int i = 0; i < chunksNumber; i++)
					{
						outFile.write(reinterpret_cast<const char *>(chunksData[i]), decompChunksLenghts[i]);
						delete[] chunksData[i];
					}
					outFile.close();
				}
			}
		}
		//unmap file ad remove it if it is specified
		unmapFile(ptr, filesize);
		
		if (REMOVE_ORIGIN)
		{
			unlink(paths[pIdx].c_str());
		}
	}
	if (!success)
	{
		printf("Exiting with (some) Error(s)\n");
		return -1;
	}
	auto endTime= std::chrono::high_resolution_clock::now();
	printf("Exiting with Success\n");
	std::cout << "execution time(s): " << (std::chrono::duration<double> (endTime - startTime)).count() << std::endl;
	return 0;
}