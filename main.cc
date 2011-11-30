/*
Ian Buitrago
Miguel Diaz
11-30-11
CS 352

Simulates an L1 cache

notes:
run with:
make ; ./cache_sim -c4 -b32 -a4 < input_trace.txt ; make clean

16 MiB ==  16 * 2^20 bytes
newest UML diagram at: http://www.gliffy.com/pubdoc/3102540/L.png
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

void run(int, int, int);

int main (int argc, char *argv[ ]){
	//the values that we get from the 
	//command line will be put into these
	//variables. we can use them later in the program
	//like for checking if they have valid values
	
	int cache_capacity = 0;
	int cache_blocksize = 0;
	int cache_associativity = 0;

	// sets params and checks format
	if(!parseParams(argc, argv, cache_capacity, cache_blocksize, cache_associativity) ) {
		exit (2);
	}
	
	if(DEBUG) cout << "Cache Capacity: " << cache_capacity << " KiB" << endl;
	if(DEBUG) cout << "Cache BlockSize: " << cache_blocksize << " Bytes" << endl;
	if(DEBUG) cout << "Cache Associativity: " << cache_associativity << " blocks per set" << endl;

	// check if commandline parameters are valid
	float capLog = log(cache_capacity)/log(2);
	float blockLog = log(cache_blocksize)/log(2);
	float assocLog = log(cache_associativity)/log(2);
	// -c <capacity> with <capacity> in KiB: 4, 8, 16, 32, or 64.
	assert(capLog == int(capLog) );		// assert power of 2
	assert(cache_capacity >= 1 && cache_capacity <= 64);
	// -b <blocksize> with <blocksize> in bytes:
	//  4, 8, 16, 32, 64, 128, 256, or 512.
	//assert(blockLog == int(blockLog) );		// assert power of 2
	assert(cache_blocksize >= 4 && cache_blocksize <= 512);
	// -a <associativity> where <associativity> is integer size of set:
	// 1, 2, 4, 8, 16. 
	assert(assocLog == int(assocLog) );		// assert power of 2
	assert(cache_associativity >= 1 && cache_associativity <= 16);
	
	run(cache_associativity, cache_blocksize, cache_capacity);
	
	return 0;
}

void run(int cache_associativity, int cache_blocksize, int cache_capacity){
	if(DEBUG) cout << "SHAZAM!" << endl;
	
	CacheMemory cache(cache_associativity, cache_blocksize, cache_capacity);

	// Input
	int read_write;
	int address;
	unsigned int data;
	if(DEBUG) cout << "Begin input:" << endl;
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
			if(DEBUG) cout << "Write: memory[" << hex << address << "] = 0x" << data << endl;
			cache.write(address, data);
		}else{		//read
			int x = cache.read(address);
			if(DEBUG) cout << "Read memory[" << hex << address << "]: 0x" << x << endl;
		}
	}
	// write all dirty lines to memory
	cache.writeDirtyBlocks();
	if(DEBUG) cout << "End of input.\n";
	
	cache.print();
	if(DEBUG) cout << "SHAZAM!" << endl;
}
