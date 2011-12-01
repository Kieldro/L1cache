#ifndef _MAIN_MEMORY_H
#define _MAIN_MEMORY_H

#define STARTING_ADDRESS 0x003f7f00
#define DEBUG false
#define DIRTY true

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
		void print();
};

#endif

