//
//buffermanager.cpp
//
//Created by alex hwang on 20/6/22
//

#include"minisql.h"
#include"BufferManager.h"
#include"CatalogManager.h"
#include<iostream>
#include<fstream>
#include<cstdio>


using namespace std;

BufferManager::BufferManager(): blockNum(0){
	Memory = new Block*[BUFFER_SIZE];
	fileChain = nullptr;
}

BufferManager::~BufferManager() {
	writeAlltoDisk();
	delete[] Memory;
}

//===========need change here=====
File* BufferManager::getFile(const char* fileName) {
	File* ftemp = fileChain;
	while (ftemp) {
		if (!strcmp(ftemp->fileName, fileName)) {
			return ftemp;
		}
        ftemp = ftemp -> nextFile;
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

Block* BufferManager::getBlock(const char* fileName, Block* block) {
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
		//writeOnetoDisk(Memory[target]); // this function actully does nothing
		Memory[target] = block;
		for (int j = 0; j < blockNum; j++) {
			Memory[j] -> time += 1; // increase the time
		}
		block -> time = 0;

	}
	return btemp;
}

Block* BufferManager::getFirstBlock(const char* fileName) {
	File* ftemp = getFile(fileName);
	if (!ftemp) { // if not found
		cerr << "File not found!" << endl;
		return nullptr;
	}
	return ftemp -> firstBlock;

}

Block* BufferManager::getNextBlock(const char* fileName, Block* block) {
	return getBlock(fileName, block->nextBlock);
}

Block* BufferManager::getLastBlock(const char* fileName) {
    Block* btemp = getFirstBlock(fileName);
    if (!btemp) { // if not found
        cerr << "Block not found!" << endl;
    }
    while (btemp -> nextBlock) {
        btemp = btemp -> nextBlock;
    }
    return btemp;
}

void BufferManager::setDirty(Block* block) {
	block -> dirty = 1;
	return;
}

void BufferManager::setPin(Block* block) {
	block -> pin = 1;
	return;
}

void BufferManager::initFile(const char* fileName, File* file) { // initialize the File
	file -> nextFile = nullptr;
	file -> firstBlock = nullptr;
	file -> fileName = (char*)fileName;
	return;
}

void BufferManager::initBlock(Block* block, const char* fileName) {
	block -> fileName = (char*)fileName;
	block -> dirty = 0;
	block -> pin = 0;
	block -> time = 0;
	block -> nextBlock = nullptr;
	block -> data = new char[BLOCK_SIZE];
	return;
}

Block* BufferManager::createBlock(const char* fileName) { // create a new block
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
	initBlock(btemp, fileName);
	return getBlock(fileName, btemp);
}

void BufferManager::appendFile(File* file) { // append the file at the end of the fileChain
	if (!fileChain) // the fileChain is empty
		fileChain = file;
	else {
        File* ftemp = fileChain;
        while (ftemp -> nextFile) {
            ftemp = ftemp -> nextFile;
        }
        ftemp -> nextFile = file;
	}
	return;

}

File* BufferManager::loadFile(const char* fileName) { // load the file from disk
    string Path = "../data/";
    string Path2(fileName);
    Path += Path2;
    ifstream in(Path);
    if (!in) { // if fails
        cerr << "No file named " << fileName << " exists" << endl;
        return nullptr;
    }
	File* ftemp = new File();
	appendFile(ftemp);
	initFile(fileName, ftemp);
	string total;
	char* temp;
	int blockSize = 0;
	int recordSize = 0;
	Block* cur;
	Block* pre;
	bool flag = true; // to indicate if it is the first block

	// compute the recordSize
	Catalog ctemp;
	recordSize = ctemp.calculateLength_attribute(fileName);
	int recordNum = BLOCK_SIZE/recordSize - 1; // compute the maximum number of records(remember to subtract the first one)
	temp = new char[recordSize];

	while (!in.eof()) {
			for (int i = 0; i < recordNum; i++) {
			if (in.eof())
				break;
			in.read(temp, recordSize);
			total.append(temp);
			blockSize += recordSize;
		}
		cur = new Block();
		initBlock(cur, fileName);
		cur -> fileName = (char*)fileName;
		cur -> data = (char*)total.c_str();
		cur -> blockSize = blockSize;
		if (flag) {
			ftemp -> firstBlock = cur;
			flag = false;
		} else
			pre -> nextBlock = cur;
		pre = cur;
		total.clear();
		recordSize = 0;
	}


	return ftemp;
}

// do not forget to update the LRU
size_t BufferManager::LRUfind() { // notice that you can only find the blocks with pin is 0
	int i = 0;
	int maxi = 0;
	for (; i < blockNum; i++) {
		if (Memory[i] -> time > Memory[maxi] -> time) {
			maxi = i;
		}
	}
	return maxi;
}

void BufferManager::writeAlltoDisk() { // write everything to disk
	File* ftemp = fileChain;
	while (ftemp) {
		writeFiletoDisk(ftemp);
		ftemp = ftemp -> nextFile;
	}
	return;

}

void BufferManager::writeFiletoDisk(File* file) { // write a file to disk
	Block* btemp = file -> firstBlock;
	while (btemp) {
		if (btemp -> dirty) // if it is dirty
			writeBlocktoDisk(btemp);
		btemp = btemp -> nextBlock;
	}
	return;
}

void BufferManager::writeBlocktoDisk(Block* block) { // write a block to disk 
	string Path("../data/");
	Path += block -> fileName;
	ofstream out(Path, ios::app);
	out << block -> data;
	out.close();
}

void BufferManager::deleteFile(const char* fileName) { // delete the file
	string Path("../data/");
	Path += fileName;
	if (!remove((char*)Path.c_str())) {
		cerr << "Deletion Faiure!" << endl;
		return;
	}

	// delete the blocks in the memory
	for (int i = 0; i < blockNum; i++) {
		if (Memory[i] -> fileName == fileName) { // if found
			Memory[i] = nullptr;
		}
	}

	// delete all the blocks the file has
	deleteBlock(fileName);

	// delete the file
	File* pre;
	File* cur;
	File* ftemp; // for deletion
	pre = cur = fileChain;
	while (cur) {
		if (cur -> fileName == fileName) { // if it is the file, delete it
			ftemp = cur;
			pre -> nextFile = cur -> nextFile;
			delete ftemp;
			return;
		} else {
			pre = cur;
		}
		cur = cur -> nextFile;
	}
}

void BufferManager::deleteBlock(const char* fileName) {
	File* ftemp = getFile(fileName);
	Block* cur = ftemp -> firstBlock;
	Block* btemp = cur;
	while (cur) {
		btemp = cur;
		cur = cur -> nextBlock;
		delete btemp;
	}
}
