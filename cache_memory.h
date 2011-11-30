#ifndef _CACHE_MEMORY_H
#define _CACHE_MEMORY_H

#include <iostream>
#include <iomanip>
#include "main_memory.h"

using namespace std;

struct CacheLine{
	int *word;
	static int blockSize;
	unsigned tag;
	bool valid;
	
	CacheLine();
	void print();
};

struct Set{
	CacheLine *line;
	static int blockSize;
	static int associativity;
	int LRU;

	Set();
	int read(unsigned tag, unsigned wordIdx, bool &found);
	void write(int data, unsigned tag, unsigned wordIdx, bool &found);
	void updateLRU();
	int getLRU();
	void print(int nSets);
};

class CacheMemory{
	private:
		Set *sets;
		int capacity, blockSize, associativity;
		int setBits, wordOffsetBits, tagBits;
		unsigned wMask, sMask, tMask;
		int hits, misses, reads, writes, evicted;		// syntactic sugar
	public: 
		MainMemory *mem;		// public for debug
		CacheMemory(int assoc, int bSize, int cap);
		~CacheMemory();
		void print();
		int read(unsigned address);
		void write(unsigned address, int data);
		void parseAddress (const unsigned address, unsigned &wordIdx, unsigned &set, unsigned &tag);
};

#endif