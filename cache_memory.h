#ifndef _CACHE_MEMORY_H
#define _CACHE_MEMORY_H

#define HEX 0
#define DEC 1
#define OCT 2

#include<iostream>

using namespace std;

class CacheMemory{
	private:
		int *memory;
		bool *valid;
		bool *dirty;
		int *tag;
		int *data;
		int size;
		int blocksize;
		int associativity;
	public: 
		CacheMemory(int capacity);
		~CacheMemory();
		void print_contents(int from, int to, int format);
		void set_content(int location, int value);
		void reset_content(void);
};

#endif

