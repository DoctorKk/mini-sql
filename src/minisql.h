//
// minisql.h
//
//
// Created by alex hwang on 20/6/19.
//

#ifndef minisql_minisql_h
#define minisql_minisql_h
#include<stdio.h>
#include<iostream>
#include<cstring>
#include<vector>

#define BLOCK_SIZE 4096

using namespace std;

struct Block {

	char* fileName; // the file the block belongs to 
	bool dirty; // whether it is dirty: 0 -> clean; 1 -> dirty
	bool pin; // the lock: 0 -> locked; 1 -> unlocked
	int time; // LRU
	int blockSize; // the size of the block
	int recordNum;
	Block* nextBlock; // next block
	char* data; // the data contained in the block
	
};
struct File {

	char* fileName; // the name of the file
	int type; // 0 -> data file
	Block* firstBlock; // the file's first block
	File* nextFile; // next file
	
};
#endif
