#ifndef _MAIN_MEMORY_H
#define _MAIN_MEMORY_H

#define HEX 0
#define DEC 1
#define OCT 2

#include<iostream>

using namespace std;

class MainMemory{
	private:
		int *memory;
		int size;
	public: 
		MainMemory(int capacity);
		~MainMemory();
		void print_contents(int from, int to, int format);
		void set_content(int location, int value);
		void reset_content(void);
};

#endif

