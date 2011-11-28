#ifndef _MAIN_MEMORY_H
#define _MAIN_MEMORY_H

#define HEX 0
#define DEC 1
#define OCT 2
#define STARTING_ADDRESS 0x003f7f00
#define DEBUG true

#include<iostream>

using namespace std;

class MainMemory{
	private:
		int *memory;
		int capacity;
	public: 
		MainMemory();
		~MainMemory();
		int read(int address);
		bool write(int address, int data);
		void reset_content(void);
		void print_contents();
};

#endif

