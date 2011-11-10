#include <iostream>
#include <new>

//*** the .cc file of a class should include its .h 
#include "main_memory.h"

using namespace std;

//*** constructor
MainMemory::MainMemory(int capacity){
     size = capacity;
     memory = new (nothrow) int [capacity];
     if (memory==0){
	cout << "Failed to allocate memory!\n";
     }
     
     for(int i = 0; i <= capacity; i++)
     {
       set_content(i, i);
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
	
	int i = from;
	while(i <= to){
	
	  cout  << hex << from << " ";		//print address first
	  
	  for(int j = 0; j < 8; j++, i++){
		  
		  switch(format){
		  case HEX:
		    cout << hex << memory[i] << " ";
		    break;
		  case DEC:
		    cout << memory[i] << " ";
		    break;
		  case OCT:
		    cout << oct << memory[i] << " ";
		    break;
		  default:
		    cout << "Invalid format";
		    i = to + 1;		//break loop
		  }
		  
		  cout  << endl;
	  }
          
	}
	cout  << endl;
}

void MainMemory::set_content(int location, int value){
	//More code
}

void MainMemory::reset_content(void){
	//More code
}
