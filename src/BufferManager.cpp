//
//buffermanager.cpp
//
//Created by alex hwang on 20/6/22
//

#include"minisql.h"
#include"BufferManager.h"

using namespace std;

BufferManager::BufferManager(): blockNum(0){
	Memory = new Block*[BUFFER_SIZE];
	fileChain = nullptr;
}

BufferManager::~BufferManager() {
	writeAlltoDisk();
	delete[] Memory;
}

File* BufferManager::getFile(char* fileName) {
	File* ftemp = fileChain;
	while (ftemp) {
		if (!strcmp(ftemp->fileName, fileName)) {
			return ftemp;
		ftemp = ftemp -> nextFile;
		}
	}
	return nullptr;
}

Block* BufferManager::getBlock(char* fileName, Block* block) {
	for (int i = 0; i < blockNum; i++) { // if it is in the memory
		if (block == Memory[i])
			return Memory[i];
	}
	// find it in the list
	File* file = getFile(fileName);
	Block* btemp = file -> firstBlock;
	while (btemp) {
		if (btemp == block)
			break;
		btemp = btemp -> nextBlock;	
	}
	if (blockNum < BUFFER_SIZE) { // if there is still space in te memory
		Memory[blockNum] = btemp;
		blockNum++;
	} else { // if there is no space in the memory
		size_t target = LRUfind();
		writeOnetoDisk(Memory[target] -> fileName, Memory[target]);
		Memory[target] = block;
	}
	return btemp;
}

Block* BufferManager::getNextBlock(char* fileName, Block* block) {
	Block* btemp = getBlock(fileName, block);
	return btemp -> nextBlock;
}

void BufferManager::setDirty(char* fileName, Block* block) {
	Block* btemp = getBlock(fileName, block);
	btemp -> dirty = 1;

}

void BufferManager::initFile(char* fileName) { // initialize the File
	File* ftemp = getFile(fileName);
	ftemp -> nextFile = nullptr;
	ftemp -> firstBlock = nullptr;
}

void BufferManager::initBlock(char* fileName, Block* block) {
	Block* btemp = getBlock(fileName, block);
	btemp -> dirty = 0;
	btemp -> pin = 0;
	btemp -> time = 0;
	btemp -> nextBlock = nullptr;
	btemp -> data = new char[BLOCK_SIZE];
}

File* createFile(char* fileName) {
	
}