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
	bool dirty;
	
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
	void print(int set);
};

class CacheMemory{
	private:
		Set *sets;
		int capacity, blockSize, associativity;
		static int setBits, wordOffsetBits, tagBits;
		int numSets;
		unsigned wMask, sMask, tMask;
		int hits, misses, reads, totalReads, writes, totalWrites, evicted;		// syntactic sugar
		void parseAddress (const unsigned address, unsigned &wordIdx, unsigned &set, unsigned &tag);
	public: 
		MainMemory *mem;		// public for debug
		CacheMemory(int assoc, int bSize, int cap);
		~CacheMemory();
		void print();
		int read(unsigned address);
		void write(unsigned address, int data);
		void writeDirtyBlocks ();
		static unsigned spliceAddress (unsigned set, unsigned tag);
};

#endif
