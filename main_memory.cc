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
	memory = new (nothrow) int [capacity];
	if (DEBUG) cout << "main mem capacity: " << capacity << " words" << endl;
	if (memory==0){
		cout << "Failed to allocate memory!\n";
	}
     
     for(int set = 0; set < capacity; ++set){
		memory[set] = set;
     }
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

void MainMemory::set_content(int address, int data){
	if(address > -1 && address < capacity && memory != NULL){
		memory[address] = data;
	}else{
		cout << "Invalid memory location or memory NULL" << endl;
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
