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
	struct {int total, reads, writes;} misses, missrate;	//temp
	int evicted = 0;
	
	cout << "STATISTICS:" << endl;
	cout << "Misses:" << endl;
	cout << "Total: " << misses.total;
	cout << " DataReads: " << misses.reads;
	cout << " DataWrites: " << misses.writes << endl;
	cout << "Miss rate: " << endl;
	cout << "Total: " << missrate.total;
	cout << " DataReads: " << missrate.reads;
	cout << " DataWrites: " << missrate.writes << endl;
	cout << "Number of Dirty Blocks Evicted From the Cache: " << evicted << endl;
	
	cout << "MAIN MEMORY:" << endl;
	//assert -1 < format < 3
	cout << "Address    Words" << endl;
	int i = from;
	while(i <= to){

	  cout  << setw(8) << setfill('0') << hex << i << "   ";		//print address first

	  for(int j = 0; j < 8; j++, i++){

		  switch(format){
		  case HEX:
		    cout << setw(8) << setfill('0') << hex << memory[i] << "   ";
		    break;
		  case DEC:
		    cout << setw(8) << setfill('0') << dec << memory[i] << "   ";
		    break;
		  case OCT:
		    cout << setw(8) << setfill('0') << oct << memory[i] << "   ";
		    break;
		  default:
		    cout << "Invalid format";
		    i = to + 1;		//break loop
		  }
	  }
	  cout  << endl;
          
	}
}

void MainMemory::set_content(int location, int value){
	//More code
}

void MainMemory::reset_content(void){
	//More code
}
