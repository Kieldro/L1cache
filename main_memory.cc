#include <iostream>
#include <iomanip>
#include <new>
#include <math.h>

//*** the .cc file of a class should include its .h 
#include "main_memory.h"


using namespace std;

//*** constructor

MainMemory::MainMemory(){
	if (DEBUG) cout << "main mem default construction.\n";
	capacity = 16 * pow(2, 20)  / 4;	// 16 MiB to 32 bit words
	// From Project specifications: You will print out the entire contents of the cache, and 1024 words of memory starting at address 0x003f7f00. 
	// should the capacity of the main memory then be 1024?
	memory = new (nothrow) int [capacity];
	if (DEBUG) cout << "main mem capacity: " << capacity << " words" << endl;
	if (memory==0){
		cout << "Failed to allocate memory!\n";
	}
	
	// initialize all memory elements to their addresses
	for(int i = 0; i < capacity; ++i){
		memory[i] = i;
	}
	if (DEBUG) cout << "constructor cap: " << capacity << endl;
	if (DEBUG) cout << "memory: " << memory << endl;
}

//*** destructor
MainMemory::~MainMemory(){
    if(memory != NULL){
		delete [] memory;
    } 
}

int MainMemory::read (int address){
	if(address > -1 && address < capacity && memory != NULL){
		return memory[address];
	}else{
		cout << "Invalid memory location or memory NULL" << endl;
	}
}

bool MainMemory::write(int address, int data){
	if(address > -1 && address < capacity && memory != NULL){
		if (DEBUG) cout << "Attempting to write..." << dec  << endl;
		if (DEBUG) cout << "cap: " << capacity << endl;
		if (DEBUG) cout << "memory[" << address << "] is " << *memory << endl;
		memory[(unsigned)address] = data;
		if (DEBUG) cout << "Successful write!" << endl;
		
		return true;		// successful
	}else{
		cout << "Invalid memory location or memory NULL" << endl;
		return false;
	}
}

void MainMemory::reset_content(void){
	if(capacity > 0 && memory != NULL){
		for(int i = 0; i < capacity; i++){
			memory[i] = 0;
	  	}
	}
}

void MainMemory::print_contents(){
	cout << "MAIN MEMORY:" << endl;
	//assert -1 < format < 3
	cout << "Address    Words" << endl;
	
	int i =  STARTING_ADDRESS;
	int to =  STARTING_ADDRESS + 1023;
	while(i <= to){
		//if(DEBUG) cout << "i = " << dec << i << endl;
		cout  << setw(8) << setfill('0') << hex << i;		//print address first

		for(int j = 0; j < 8; j++, i++){
			cout << "   " << setw(8) << setfill('0');
			switch(HEX){
				case HEX:
					cout << hex;
					break;
				case DEC:
					cout << dec ;
					break;
				case OCT:
					cout << oct;
					break;
				default:
					cout << "Invalid format";
					i = to + 1;		//break loop
			}
			 cout << memory[i];
		}
		cout  << endl;         
	}
}
