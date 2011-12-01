#include <iostream>
#include <iomanip>
#include <new>
#include <math.h>

//*** the .cc file of a class should include its .h 
#include "main_memory.h"

using namespace std;


//*** constructor
MainMemory::MainMemory(){
	if(DEBUG) cout << "CONSTRUCTING MainMemory obj: " << this << endl;
	capacity = 16 * pow(2, 20)  / 4;	// 16 MiB to 32 bit words
	memory = new (nothrow) int [capacity];
	if (DEBUG) cout << "main mem capacity: " << capacity << " words" << endl;
	if (memory==0){
		cout << "Failed to allocate memory!\n";
	}
	
	// initialize all memory elements to their addresses
	for(int i = 0; i < capacity; ++i){
		memory[i] = i;
	}
}

//*** destructor
MainMemory::~MainMemory(){
    if(memory != NULL){
		delete [] memory;
    }
	if (DEBUG) cout << "DESTRUCTED MainMemory obj: " << this << endl;
}

int MainMemory::read (int address){
	if(address > -1 && address < capacity && memory != NULL){
		return memory[address];
	}else
		cout << "Invalid memory location or memory NULL" << endl;
}

bool MainMemory::write(int address, int data){
	if(address > -1 && address < capacity && memory != NULL){
		memory[(unsigned)address] = data;
		
		return true;		// successful
	}else{
		cout << "Invalid memory location or memory NULL" << endl;
		if (DEBUG) cout << "memory[" << address << "] is " << memory << endl;
		if (DEBUG) cout << "cap: " << capacity << endl;
		
		return false;
	}
}

void MainMemory::print(){
	cout << "MAIN MEMORY:" << endl;
	//assert -1 < format < 3
	cout << "Address    Words" << endl;
	
	int to =  STARTING_ADDRESS + 1023;
	for(int i =  STARTING_ADDRESS; i < to; i=i+8){
		cout << setw(8) << setfill('0') << hex << i;
		for(int j = 0; j < 8; j++)
			cout << "   " << setw(8) << setfill('0') << memory[i+j];
		
		cout << endl;
	}
}
