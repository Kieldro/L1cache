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
	unsigned tag;
	bool valid;
	bool dirty;
	int *word;
	cacheLine(){valid = dirty = 0;}
};

struct Set{
	cacheLine *line;
	int blockSize;

	//Set();
	void initialize(int);
	int read(unsigned tag, unsigned wordIdx);
	void print();
};

class CacheMemory{
	private:
		Set *sets;
		int *memory;	// dummy var
		int capacity;
		int blockSize;
		int associativity;
		float setBits;
		float wordOffsetBits;
		int tagBits;
		unsigned wMask, sMask, tMask;
		int hits, misses, reads, writes, evicted;
	public: 
		MainMemory *mem;		// public for debug
		CacheMemory(int assoc, int bSize, int cap);
		~CacheMemory();
		void print_contents();
		void write(unsigned address, int data);
		int read(unsigned address);
		void parseAddress (unsigned &address, unsigned &wordIdx, unsigned &set, unsigned &tag);
		void reset_content(void);
};
#endif

