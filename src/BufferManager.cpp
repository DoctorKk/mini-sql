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
	// when not found
	ftemp = loadFile(fileName); // load the file to the fileChain
	if (!ftemp) { // file not found on the disk
		cerr << "File not found on the disk!" << endl;
		return nullptr;
	}
	appendFile(ftemp); // append the file 

	return ftemp;
}

Block* BufferManager::getBlock(char* fileName, Block* block) {
	for (int i = 0; i < blockNum; i++) { // if it is in the memory
		if (block == Memory[i]) {
			for (int j = 0; j < blockNum; j++) {
				Memory[j] -> time += 1; // increase the time
			}
			block -> time = 0;
			return Memory[i];

		}
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
		writeOnetoDisk(Memory[target] -> fileName, Memory[target]); // this function actully does nothing
		Memory[target] = block;
		for (int j = 0; j < blockNum; j++) {
			Memory[j] -> time += 1; // increase the time
		}
		block -> time = 0;

	}
	return btemp;
}

Block* BufferManager::getNextBlock(char* fileName, Block* block) {
	return getBlock(fileName, block->nextBlock);
}

void BufferManager::setDirty(Block* block) {
	block -> dirty = 1;
	return;
}

void BufferManager::setPin(Block* block) {
	block -> pin = 1;
	return;
}

void BufferManager::initFile(char* fileName) { // initialize the File
	File* ftemp = getFile(fileName);
	ftemp -> nextFile = nullptr;
	ftemp -> firstBlock = nullptr;
}

void BufferManager::initBlock(Block* block) {
	block -> dirty = 0;
	block -> pin = 0;
	block -> time = 0;
	block -> nextBlock = nullptr;
	block -> data = new char[BLOCK_SIZE];
	return;
}

Block* BufferManager::createBlock(char* fileName) { // create a new block
	File* ftemp = getFile(fileName);
	if (!ftemp) { // if not found
		cerr << "File not found!" << endl;
		return nullptr;
	}		
	Block* btemp = new Block;
	Block* ptr = ftemp -> firstBlock; // the first block of the file
	while (ptr -> nextBlock != nullptr) {
		ptr = ptr -> nextBlock;
	}
	ptr -> nextBlock = btemp;
	initBlock(btemp);
	return getBlock(fileName, btemp);
}

void BufferManager::appendFile(File* file) { // append the file at the end of the fileChain
	if (!fileChain) // the fileChain is empty
		fileChain = file;
	File* ftemp = fileChain;
	while (ftemp -> nextFile) {
		ftemp = ftemp -> nextFile;
	}
	ftemp -> nextFile = file;
	return;

}


// do not forget to update the LRU

size_t BufferManager::LRUfind() { // notice that you can only find the blocks with pin is 0

}

void BufferManager::writeAlltoDisk() { // write everything to disk

}

void BufferManager::writeOnetoDisk(char* fileName, Block* block) { // this 
	return;
}