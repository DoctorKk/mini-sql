// 
// buffermanger.h
//
// Created by alex hwang on 20/6/20
//

#ifndef minisql_BufferManager_h
#define minisql_BufferManager_h
#include"minisql.h"

#define BUFFER_SIZE 40 // the maximum blocks the buffer can contain

using namespace std;

class BufferManager {
public:
	BufferManager();
	~BufferManager();
	File* getFile(char* fileName); // fetch the file
	File* createFile(char* fileName); // append a new file at the end of the list
	Block* getBlock(char* fileName, Block* block); // fetch the block
	Block* getNextBlock(char* fileName, Block* block); // fetch the next block
	Block* createBlock(char* fileName); // append a new block at the end of the File
	void writeOnetoDisk(char* fileName, Block* block); // write the block to the disk 
	void writeAlltoDisk(); // write all the blocks in the memory to the disk 
	void setDirty(char* fileName, Block* block); // set the block's dirty to 1
	void setPin(char* fileName, Block* block); // pin the block in the memory
	size_t LRUfind(); // find the block to be unloaded
	void initFile(char* fileName);
	void initBlock(char* fileName, Block* block);

private:
	int blockNum; // the number of blocks already loaded onto the memory
	Block** Memory; // the virtual memory
	File* fileChain; // all the file the system has
};
#endif
