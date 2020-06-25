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

	// the functions record manager can use	
	File* getFile(char* fileName); // fetch the file
	Block* getFirstBlock(char* fileName, Block* block); // fetch the first block of the file
	Block* getNextBlock(char* fileName, Block* block); // fetch the next block
	Block* getLastBlock(char* fileName); // fetch the last block of the file
	void setDirty(Block* block); // set the block's dirty to 1
	void setPin(Block* block); // pin the block in the memory
	Block* createBlock(char* fileName); // append a new block at the end of the File

	void writeBlocktoDisk(Block* block); // write the block to the disk 
	void writeAlltoDisk(); // write all the blocks in the memory to the disk 
	void writeFiletoDisk(File* file); // write the file to disk
	File* loadFile(char* fileName); // load file from the disk
	size_t LRUfind(); // find the block to be unloaded
	void initFile(char* fileName);
	void initBlock(Block* block);
	void appendFile(File* file);

private:
	Block* getBlock(char* fileName, Block* block); // fetch the block
	int blockNum; // the number of blocks already loaded onto the memory
	Block** Memory; // the virtual memory
	File* fileChain; // all the file the system has
};
#endif
