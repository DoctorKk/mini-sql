// 
// buffermanger.h
//
// Created by alex hwang on 20/6/20
//

#ifndef minisql_BufferManager_h
#define minisql_BufferManager_h
#include"minisql.h"

#define BUFFER_SIZE 80 // the maximum blocks the buffer can contain

using namespace std;

class BufferManager {
public:
	BufferManager();
	~BufferManager();

	// the functions record manager can use	
	File* getFile(const char* fileName); // fetch the file
	Block* getFirstBlock(const char* fileName); // fetch the first block of the file
	Block* getNextBlock(const char* fileName, Block* block); // fetch the next block
	Block* getLastBlock(const char* fileName); // fetch the last block of the file
	void setDirty(Block* block); // set the block's dirty to 1
	void setPin(Block* block); // pin the block in the memory
	Block* createBlock(const char* fileName); // append a new block at the end of the File
	void deleteFile(const char* fileName); // delete file

	void writeBlocktoDisk(Block* block); // write the block to the disk 
	void writeFiletoDisk(File* file); // write the file to disk
	void writeAlltoDisk(); // write everything in the memory to the disk 
	File* loadFile(const char* fileName); // load file from the disk
	size_t LRUfind(); // find the block to be unloaded
	void initFile(const char* fileName, File* file);
	void initBlock(Block* block, const char* fileName);
	void appendFile(File* file);
	void deleteBlock(const char* fileName); // delete all the blocks the file has

private:
	Block* getBlock(const char* fileName, Block* block); // fetch the block
	int blockNum; // the number of blocks already loaded onto the memory
	Block** Memory; // the virtual memory
	File* fileChain; // all the file the system has
};
#endif
