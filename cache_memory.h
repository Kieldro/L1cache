#ifndef _CACHE_MEMORY_H
#define _CACHE_MEMORY_H

#define HEX 0
#define DEC 1
#define OCT 2

#include <iostream>
#include <iomanip>
#include "main_memory.h"

using namespace std;

struct cacheLine{
	int *word;
	unsigned tag;
	bool valid;
	bool dirty;
	cacheLine(){ valid = dirty = 0; }
};

struct Set{
	cacheLine *line;
	static int blockSize;
	static int associativity;

	Set();
	int read(unsigned tag, unsigned wordIdx);
	void print();
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