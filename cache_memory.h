#ifndef _CACHE_MEMORY_H
#define _CACHE_MEMORY_H

#define HEX 0
#define DEC 1
#define OCT 2

#include<iostream>
#include <iomanip>

using namespace std;

class CacheMemory{
	private:
		int *memory;
		int set;
		bool *valid;
		int *tag;
		bool *dirty;
		int *data;
		int capacity;
		int blocksize;
		int associativity;
	public: 
		CacheMemory(int capacity, int b, int c);
		~CacheMemory();
		void print_contents(int from, int to, int format);
		void set_content(int location, int value);
		void reset_content(void);
};
#endif

