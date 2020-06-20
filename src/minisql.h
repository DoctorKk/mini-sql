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

#define BLOCK_SIZE 4096

using namespace std;

struct File {

	char* address; // the address of the file
	int type; // 0 -> data file; 1 -> index file; 2 -> catalog file
	int blockNum; // the number of blocks the file has
	int recordNum; // the number of records the file has
	bool pin; // the lock: 0 -> locked; 1 -> unlocked
	Block* firstBlock; // the file's first block
	File* nextFile; // next file
	
}

struct Block {

	int offset; // the number of the block 
	bool dirty; // whether it is dirty: 0 -> clean; 1 -> dirty
	bool pin; // the lock: 0 -> locked; 1 -> unlocked
	int recordNum; // the number of records the block has
	int time; // LRU
	Block* nextBlock; // next block
	File* file; // the file it belongs to
	int freeSize; // the available size of the block
	char* data; // the data contained in the block
	
}
