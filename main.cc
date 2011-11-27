/*
Ian Buitrago
Miguel Diaz

notes:
run with
make ; ./cache_sim -c8 -b16 -a4 < input_trace.txt

16 MB ==  16 * 1048576 bytes || 16 *1000000 bytes?
*/

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "funcs.h"
#include "main_memory.h"
#include "cache_memory.h"

#define CACHE_READ 0
#define CACHE_WRITE 1

using namespace std;

void run();

int main (int argc, char *argv[ ])
{
	//the values that we get from the 
	//command line will be put into these
	//variables. we can use them later in the program
	//like for checking if they have valid values
	
	int cache_capacity = 0;
	int cache_blocksize = 0;
	int cache_associativity = 0;
	
	if(!parseParams(argc, argv, cache_capacity, cache_blocksize, cache_associativity) ) {
		exit (2);
	}
	
	cout << "Cache Capacity: " << cache_capacity << " KiB" << endl;
	cout << "Cache BlockSize: " << cache_blocksize << " Bytes" << endl;
	cout << "Cache Associativity: " << cache_associativity << " blocks per set" << endl;

	// check if commandline parameters are valid
	float capLog = log(cache_capacity)/log(2);
	float blockLog = log(cache_blocksize)/log(2);
	float assocLog = log(cache_associativity)/log(2);
	// -c <capacity> with <capacity> in KiB: 4, 8, 16, 32, or 64.
	assert(capLog == int(capLog) );		// assert power of 2
	assert(cache_capacity >= 4 || cache_capacity <= 64);
	// -b <blocksize> with <blocksize> in bytes:
	//  4, 8, 16, 32, 64, 128, 256, or 512.
	assert(blockLog == int(blockLog) );		// assert power of 2
	assert(cache_blocksize >= 4 || cache_blocksize <= 512);
	// -a <associativity> where <associativity> is integer size of set:
	// 1, 2, 4, 8, 16. 
	assert(assocLog == int(assocLog) );		// assert power of 2
	assert(cache_associativity >= 1 || cache_associativity <= 16);
	
	run();
	
	MainMemory mem;
	CacheMemory cache(mem, cache_associativity, cache_blocksize, cache_capacity);
	
	int read_write;
	int address;
	unsigned int data;
	if(DEBUG) cout << "beginning input." << endl;
	// repeat till we reach the end of the input	
	while(!feof(stdin)){
	  	//read in whether to read or write to the cache
		cin >> dec >> read_write;
		
		// check again if we have reached the end
		// as this flag is set only after a 'cin'
		if(feof(stdin)) break;
		
		cin >> hex >> address;

		//if it is a cache write, then we have to read the data
		if(read_write == CACHE_WRITE){
		  	cin >> hex >> data;
			if(DEBUG) cout << "Write: memory[" << hex << address << "] = " << data << endl;
			mem.set_content(address, data);		//write to memory	
		}else{		//read
			if(DEBUG) cout << "Read memory[" << hex << address << "]: " << cache.read(address) << endl;
		}
	}
	
	//cache.print_contents();
	
	//mem.print_contents(STARTING_ADDRESS, STARTING_ADDRESS + 1023, HEX);
	
	run();
	
	return 0;
}

void run(){
		if(DEBUG) cout << "SHAZAM!\n";
}
