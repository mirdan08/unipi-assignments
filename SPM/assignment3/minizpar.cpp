#include <config.hpp>
#include <cmdline.hpp>
#include <utility.hpp>
#include <vector>
#include <cmath>
#include <iostream>

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

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		usage(argv[0]);
		return -1;
	}
	long start = parseCommandLine(argc, argv);

	
	size_t blockSize = 1000;
	bool success = true;
	for (int fIdx = start; fIdx < argc; fIdx++)
	{
		size_t filesize = 0;
		if (isDirectory(argv[fIdx], filesize))
		{
				success &= walkDir(argv[fIdx], COMP);
		}
		else
		{
			// load file by mapping it to the memory
			unsigned char *ptr = nullptr;
			if (!mapFile(argv[fIdx], filesize, ptr))
			{
				if (QUITE_MODE >= 1)
				std::fprintf(stderr, "mapFile %s failed\n", argv[fIdx]);
				success=false;
				break;
			}
			
			if (COMP)
			{
				size_t chunksNumber = (filesize + blockSize - 1) / blockSize;
				
				std::vector<size_t> chunksLenghts(chunksNumber);
				std::vector<unsigned char *> chunksData(chunksNumber);
				#pragma omp parallel shared(blockSize) \
					shared(argv)                       \
					shared(fIdx)                       \
					shared(success)					   \
					shared(ptr) \
					shared(chunksLenghts) \
					shared(chunksData) \

				{
					#pragma omp for reduction(&& : success) 
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
						std::string fname = std::string(argv[fIdx]) + ".pzip";
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
					#pragma omp parallel shared(ptr,chunksIdx,chunksLenghts,chunksData,decompChunksLenghts,chunksNumber) 
					{
						#pragma omp for reduction(&& : success) 
						for (int i = 0; i < chunksNumber; i++)
						{
							success &= decompressChunkData(
								&ptr[chunksIdx[i]], chunksLenghts[i],
								&chunksData[i], decompChunksLenghts[i]);
						}
						
						#pragma omp single 
						{

							std::ofstream outFile(argv[fIdx], std::ios::binary);
							for (int i = 0; i < chunksNumber; i++)
							{
								outFile.write(reinterpret_cast<const char *>(chunksData[i]), decompChunksLenghts[i]);
							}
							outFile.close();
						}
					}

				}

				unmapFile(ptr, filesize);
			}
	}
	if (!success)
	{
		printf("Exiting with (some) Error(s)\n");
		return -1;
	}
	printf("Exiting with Success\n");
	return 0;
}