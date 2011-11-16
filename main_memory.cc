#include <iostream>
#include <iomanip>
#include <new>

//*** the .cc file of a class should include its .h 
#include "main_memory.h"

#define STARTING_ADDRESS 4161280 // 0x003f7f00

using namespace std;

//*** constructor
MainMemory::MainMemory(int capacity){
     size = capacity;
     memory = new (nothrow) int [1024];
     if (memory==0){
	cout << "Failed to allocate memory!\n";
     }
     
     for(int set = 0; set < 1024; set++)
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
	//assert -1 < format < 3
	cout << "Address    Data" << endl;
	while(from <= to)
	  {
		if(format == 0)
		{
		  cout << setw(8) << setfill('0') << hex << STARTING_ADDRESS + from << "   " << hex << memory[from] << endl;
		}
		if(format == 1)
		{
		  cout << setw(8) << setfill('0') << hex << STARTING_ADDRESS + from << "   " << hex << memory[from] << endl;
		}
		if(format == 2)
		{
		  cout << setw(8) << setfill('0') << hex << STARTING_ADDRESS + from << "   " << hex << memory[from] << endl;
		}
		if(format < 0 || format > 2)
		{
		  cout << "Invalid format";
		  from = to + 1; // exit loop early
		}
		from++;
	  }
}

void MainMemory::set_content(int location, int value){
	//More code
}

void MainMemory::reset_content(void){
	//More code
}
