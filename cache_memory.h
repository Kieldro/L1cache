#ifndef _CACHE_MEMORY_H
#define _CACHE_MEMORY_H

#define HEX 0
#define DEC 1
#define OCT 2

#include <iostream>
#include <iomanip>
#include "main_memory.h"

using namespace std;

struct Set{
	int *cacheLine;		// array of blocks
	int blockSize;

	//Set();
	void initialize(int);
	void print();
};

class CacheMemory{
	private:
		int *memory;
		Set *sets;
		bool *valid;
		int *tag;
		bool *dirty;
		int *data;
		int capacity;
		int blockSize;
		int associativity;
		float setBits;
		float wordOffsetBits;
		int tagBits;
		unsigned wMask;
		unsigned sMask;
		unsigned tMask;
		int hits;
		int misses;
		int reads;
		int writes;
		int evicted;
	public: 
		MainMemory *mem;
		CacheMemory(MainMemory, int a, int bsize, int c);
		~CacheMemory();
		void print_contents();
		void set_content(int location, int value);
		int read(unsigned address);
		void reset_content(void);
};
#endif

