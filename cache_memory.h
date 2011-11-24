#ifndef _CACHE_MEMORY_H
#define _CACHE_MEMORY_H

#define HEX 0
#define DEC 1
#define OCT 2

#include<iostream>
#include <iomanip>
#include "main_memory.h"

using namespace std;

class CacheMemory{
	private:
		MainMemory *mem;
		int *memory;
		int set;
		bool *valid;
		int *tag;
		bool *dirty;
		int *data;
		int capacity;
		int blocksize;
		int associativity;
		int hits;
		int misses;
		int reads;
		int writes;
		int evicted;
	public: 
		CacheMemory(MainMemory, int a, int bsize, int c);
		~CacheMemory();
		void print_contents(int from, int to, int format);
		void set_content(int location, int value);
		void reset_content(void);
};
#endif

