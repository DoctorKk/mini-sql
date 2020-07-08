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
	    string Path = "../data/";
	    Path += fileName;
	    fstream in(Path, ios::out);
	    in.close();
	    ftemp = loadFile(fileName);
	}
	appendFile(ftemp); // append the file 

	return ftemp;
}

Block* BufferManager::getBlock(const char* fileName, Block* block) {
	/*for (int i = 0; i < blockNum; i++) { // if it is in the memory
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
	 */
	return block;
}

Block* BufferManager::getFirstBlock(const char* fileName) {
	File* ftemp = getFile(fileName);
	if (!ftemp) { // if not found
		cerr << "File not found!" << endl;
		return nullptr;
	}
	return getBlock(fileName, ftemp -> firstBlock);

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
	file -> type = 0;
	file -> fileName = new char[strlen(fileName)+1];
	strcpy(file->fileName, fileName);
	return;
}

void BufferManager::initBlock(Block* block, const char* fileName) {
	block -> fileName = new char[strlen(fileName)+1];
	strcpy(block->fileName, fileName);
	block -> dirty = 0;
	block -> pin = 0;
	block -> time = 0;
	block -> nextBlock = nullptr;
	block -> blockSize = 0;
	block -> data = new char[BLOCK_SIZE + 1];
	return;
}

Block* BufferManager::createBlock(const char* fileName) { // create a new block
	File* ftemp = getFile(fileName);
	if (!ftemp) { // if not found
		cerr << "File not found!" << endl;
		return nullptr;
	}		
	Block* btemp = new Block;
	Block* ptr = getLastBlock(fileName);
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
//============use strcpy here
File* BufferManager::loadFile(const char* fileName) { // load the file from disk
    string Path = "../data/";
    string Path2(fileName);
    Path += Path2;
    ifstream in(Path);
    if (!in) { // if fails
        return nullptr;
    }
	File* ftemp = new File();
    initFile(fileName, ftemp);
	//appendFile(ftemp);
	string temp;
    string total;
    int blockSize = 0;
	Block* cur;
	Block* pre;
	bool flag = true; // to indicate if it is the first block
	bool flag2 = true;

	// compute the length of the file
	in.seekg(0, in.end);
	int length = in.tellg();
	in.seekg(0, in.beg);

	if (length==0) { // nothing in the file
	    cur = new Block;
	    initBlock(cur, fileName);
	    ftemp -> firstBlock = cur;
	    return ftemp;
	}
	//temp = new char[recordSize];

	int totalNum = 0; // the size already taken
	while (!in.eof()) {
	    total = new char[BLOCK_SIZE + 1];
        while (blockSize < BLOCK_SIZE) {
            if (in.eof())
                break;
	        getline(in, temp);
	        if (temp.empty())
	            break;
			if (blockSize + temp.size() > BLOCK_SIZE)
	            break;
	        totalNum++;
	        if (!flag2) {
	            total.append("\n");
	        } else {
	            flag2 = false;
	        }
			total.append(temp);
			blockSize += temp.size() + 1;
	    }
		cur = new Block;
		initBlock(cur, fileName);
		flag2 = true;
		total.append("\n");
		//strcpy(cur -> fileName, (char*)fileName);
		strcpy(cur -> data, (char*)total.c_str());
        //cur -> data = (char*) total.c_str();
		cur -> blockSize = blockSize;
		cur -> recordNum = totalNum;
		if (flag) {
			ftemp -> firstBlock = cur;
			flag = false;
		} else
			pre -> nextBlock = cur;
		pre = cur;
		//total.clear();
		blockSize = 0;
	}
    in.close();
    //writeFiletoDisk(ftemp);
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
    string Path = "../data/";
    Path += file -> fileName;
    ofstream out(Path, ios::out);
    out.close();
	Block* btemp = file -> firstBlock;
	while (btemp) {
		//if (btemp -> dirty) // if it is dirty
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
