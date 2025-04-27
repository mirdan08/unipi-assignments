#include <config.hpp>
#include <cmdline.hpp>
#include <utility.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <cstdio>
#include <filesystem>
#include <chrono>

/*
In this implementation we need a header for the compression part as
we have to memorize at least the singly compressed blocks
*/

static inline bool compressChunkData(
	unsigned char *startPtr,
	size_t size,
	// output params
	size_t &chunk_len,
	unsigned char **chunkPtr)
{

	unsigned char *inPtr = startPtr;
	size_t inSize = size;
	// get an estimation of the maximum compression size
	size_t cmp_len = compressBound(inSize);
	// allocate memory to store compressed data in memory
	unsigned char *ptrOut = new unsigned char[cmp_len];
	if (compress(ptrOut, &cmp_len, (const unsigned char *)inPtr, inSize) != Z_OK)
	{
		return false;
	}
	// save output params
	chunk_len = cmp_len;
	*chunkPtr = ptrOut;
	return true;
}

static inline bool decompressChunkData(
	unsigned char *chunkStartPtr,
	size_t chunk_size,
	// output params
	unsigned char **decompChunkPtr,
	size_t &decompChunkLen)
{
	size_t decompressedSize = chunk_size; // read the original size

	// Write the decompressed data to a file
	unsigned char *decompressed_data = new unsigned char[chunk_size];
	// decompress the data
	if (uncompress(decompressed_data, &decompressedSize, chunkStartPtr, chunk_size) != Z_OK)
	{
		return false;
	}
	decompChunkLen = decompressedSize;
	*decompChunkPtr = decompressed_data;
	return true;
}

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
					
					#pragma omp single
					{
						std::string fname = std::string(paths[pIdx]) + ".pzip";
						std::ofstream outFile(fname, std::ios::binary);
						// write number of chunks
						outFile.write(reinterpret_cast<const char *>(&chunksNumber), sizeof(size_t));
						// write chunks lengths
						for (size_t i = 0; i < chunksNumber; i++)
						{
							outFile.write(reinterpret_cast<const char *>(&chunksLenghts[i]), sizeof(size_t));
						}
					// write actual chunkData
					for (size_t i = 0; i < chunksNumber; i++)
					{
						outFile.write(reinterpret_cast<const char *>(chunksData[i]), sizeof(unsigned char) * chunksLenghts[i]);
						delete[] chunksData[i];
					}
					outFile.close();
				}
			}
		}
		else
		{

			size_t chunksNumber;
			std::memcpy(&chunksNumber, ptr, sizeof(size_t));
			size_t offset = sizeof(size_t);
			std::vector<size_t> decompChunksLenghts(chunksNumber);
			std::vector<size_t> chunksIdx(chunksNumber);
			std::vector<size_t> chunksLenghts(chunksNumber);
			std::vector<unsigned char *> chunksData(chunksNumber);
			
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