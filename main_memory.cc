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
}

//*** destructor
MainMemory::~MainMemory(){
     if(memory != NULL){
	delete [] memory;	
     } 
}

void MainMemory::print_contents(int from, int to, int format){
	//Some code to print the memory content 
}

void MainMemory::set_content(int location, int value){
	//More code
}

void MainMemory::reset_content(void){
	//More code
}

