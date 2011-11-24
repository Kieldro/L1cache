#include <iostream>
#include <iomanip>
#include <new>

//*** the .cc file of a class should include its .h 
#include "main_memory.h"


using namespace std;

//*** constructor

MainMemory::MainMemory(){
	if (DEBUG) cout << "Default constructoion.\n";
	capacity = MEM_CAPACITY;
	memory = new (nothrow) int [capacity];
	if (memory==0){
		cout << "Failed to allocate memory!\n";
	}
     
     for(int set = 0; set < capacity; set++)
     {
		memory[set] = STARTING_ADDRESS + set;
     }
}

//*** destructor
MainMemory::~MainMemory(){
    if(memory != NULL){
		delete [] memory;
    } 
}

void MainMemory::print_contents(int from, int to, int format){
	cout << "MAIN MEMORY:" << endl;
	//assert -1 < format < 3
	cout << "Address    Words" << endl;
	
	int i = 0;
	to = 1023;
	while(i <= to){
		//if(DEBUG) cout << "i = " << dec << i << endl;
		cout  << setw(8) << setfill('0') << hex << i + STARTING_ADDRESS;		//print address first

		for(int j = 0; j < 8; j++, i++){
			cout << "   " << setw(8) << setfill('0');
			switch(format){
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

void MainMemory::set_content(int location, int value){
	if(location > -1 && location < capacity && memory != NULL){
		memory[location] = value;
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
