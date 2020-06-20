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
	File getFile(char* fileAddress); // fetch the file
	Block getBlock(char* fileAddress, int offset); // fetch the block
	bool findBlock(char* fileAddress, int offset); // find the block in the memory
	void loadBlock(char * fileAddress, int offset); // load the block from disk to memory
	void unloadBlock(int des); // unload the des'th block to disk
	size_t LRUfind(); // find the block to be unloaded

private:
	int blockNum; // the number of blocks already loaded onto the memory
	size_t curBlock; // the current block in the memory
	vector<Block> Memory(BUFFER_SIZE); // the virtual memory
}
