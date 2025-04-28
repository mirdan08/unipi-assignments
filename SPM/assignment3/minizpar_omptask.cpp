#include <config.hpp>
#include <cmdline.hpp>
#include <utility.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <cstdio>
#include <filesystem>
#include <chrono>
#include <omp.h>

//compress a single chunk
static inline bool compressChunkData(
	unsigned char *startPtr,
	size_t size,
	// output params
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

//decompress a single chunk
static inline bool decompressChunkData(
	unsigned char *chunkStartPtr,
	size_t chunk_size,
	// output params
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
//remove suffix from the file name
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
	int fIdx = start;

	//read files paths on case of folders
	while (argv[start])
	{
		if (std::filesystem::exists(argv[fIdx]))
		{
			if (std::filesystem::is_directory(argv[fIdx]))
			{
				if (RECUR)
				{
					for (const auto &entry : std::filesystem::recursive_directory_iterator(argv[fIdx]))
					{
						if (entry.is_regular_file())
						{
							paths.push_back(entry.path());
						}
					}
				}
				else
				{
					for (const auto &entry : std::filesystem::directory_iterator(argv[fIdx]))
					{
						if (entry.is_regular_file())
						{
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
	auto startTime = std::chrono::high_resolution_clock::now();
	//begin instantiating taks
	#pragma omp parallel
	{
		#pragma omp single
		{
			for (int pIdx = 0; pIdx < paths.size(); pIdx++)
			{	
				//instantiate a taskgroup
				#pragma omp taskgroup
				{

					size_t filesize = 0;
					unsigned char *ptr = nullptr;
					//map file to memory
					#pragma omp task shared(ptr,filesize)
					{
						if (!mapFile(paths[pIdx].c_str(), filesize, ptr))
						{
							if (QUITE_MODE >= 1)
							std::fprintf(stderr, "mapFile %s failed\n", paths[pIdx]);
							success = false;
						}
					}

					#pragma omp taskwait
					//instanti
					if (COMP)
					{
						size_t chunksNumber = (filesize + blockSize - 1) / blockSize;
						
						std::vector<size_t> chunksLenghts(chunksNumber);
						std::vector<unsigned char *> chunksData(chunksNumber);
						//main source of parallelism for compression on a single file
						#pragma omp taskloop shared(chunksLenghts,chunksData,ptr)
						for (int i = 0; i < chunksNumber; i++)
						{
							unsigned char *ptrStart = ptr + i * blockSize;
							
							size_t blockOffset = i * blockSize;
							size_t chunksSize = std::min(blockSize, filesize - blockOffset);
							
							size_t compChunkLen = -1;
							unsigned char *compressedChunk = nullptr;
							#pragma omp atomic
							success &= compressChunkData(
								ptrStart, chunksSize,
								compChunkLen, &compressedChunk);
							chunksLenghts[i] = compChunkLen;
							chunksData[i] = compressedChunk;
								
						}
						
						#pragma omp taskwait
						//write the result on the disk with its header
						#pragma omp task shared(chunksLenghts,chunksData,chunksNumber)
						{
							std::string fname = std::string(paths[pIdx]) + ".pzip";
							std::ofstream outFile(fname, std::ios::binary);
							// write number of chunks
							outFile.write(reinterpret_cast<const char *>(&chunksNumber), sizeof(size_t));
							// write chunks lengths
							outFile.write(reinterpret_cast<const char *>(chunksLenghts.data()), chunksNumber*sizeof(size_t));
							// write actual chunkData
							for (size_t i = 0; i < chunksNumber; i++)
							{
								//std::cout << chunksData[i]<< std::endl;
								outFile.write(reinterpret_cast<const char *>(chunksData[i]), sizeof(unsigned char) * chunksLenghts[i]);
								delete[] chunksData[i];
							}
							outFile.close();
						}

						#pragma omp taskwait
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
						//extract and elaborate the header
						#pragma omp task shared(chunksIdx, chunksLenghts, chunksData, offset, ptr)
						{
							// std::cout<<  "th: " <<omp_get_thread_num() << " is reading the header for " << paths[pIdx] << std::endl;
							
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
						}
						
						#pragma omp taskwait
						//main source of parallelism for decompression on a single file
						#pragma omp taskloop shared(decompChunksLenghts, ptr, chunksData, success)
						for (int i = 0; i < chunksNumber; i++)
						{
							size_t expectedSize = 0; 
							unsigned char *outChunk = nullptr;
							#pragma omp atomic
							success &= decompressChunkData(
								&ptr[chunksIdx[i]], chunksLenghts[i],
								&outChunk, expectedSize);
								chunksData[i] = outChunk;
								decompChunksLenghts[i] = expectedSize;
						}
								
						#pragma omp taskwait
						//write results to the disk
						#pragma omp task shared(chunksData, decompChunksLenghts)
						{
							std::ofstream outFile(removeSuffix(std::string(paths[pIdx]), ".pzip"), std::ios::binary);
							for (int i = 0; i < chunksNumber; i++)
							{
								outFile.write(reinterpret_cast<const char *>(chunksData[i]), decompChunksLenghts[i]);
								delete chunksData[i];
							}
							outFile.close();
						}
						#pragma omp taskwait
					}

					#pragma omp taskwait
					//file is not needed anymore unmap it and delete it if needed
					#pragma omp task shared(ptr, filesize, paths, pIdx)
					{
						unmapFile(ptr, filesize);
						
						if (REMOVE_ORIGIN)
						{
							unlink(paths[pIdx].c_str());
						}
					}
					#pragma omp taskwait
				}
			}
		}
	}
	
	if (!success)
	{
		printf("Exiting with (some) Error(s)\n");
		return -1;
	}
	auto endTime = std::chrono::high_resolution_clock::now();
	printf("Exiting with Success\n");
	std::cout << "execution time(s): " << (std::chrono::duration<double>(endTime - startTime)).count() << std::endl;
	return 0;
}