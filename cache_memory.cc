#include <iostream>
#include <iomanip>
#include <new>
#include <math.h>

//*** the .cc file of a class should include its .h 
//T I B
//B = word index within block, 
#include "cache_memory.h"

using namespace std;

CacheMemory::CacheMemory(MainMemory m, int a, int b, int c){
	capacity = c * pow(2, 10) / 4;		// convert KiB to words
	mem = &m;		//link to a MainMemory object
	associativity = a;
	blockSize = b / 4;		// convert from bytes to words
	//int totalBlocks = capacity;
	hits = misses = writes = reads = evicted = 0;
	
	const int numSets = capacity / blockSize / associativity;
	//assert(divides nicely);
	if(DEBUG) cout << "cache capacity: " << capacity << " words" << endl;
	if(DEBUG) cout << "block size: " << blockSize << " words" << endl;
	if(DEBUG) cout << "number of sets: " << numSets << " sets" << endl;
	float setBits = log(numSets)/log(2);
	float wordOffsetBits = log(blockSize)/log(2);
	int tagBits = 32 - setBits - wordOffsetBits;
	if(DEBUG) cout << "bits for set index: " << setBits << " bits" << endl;
	if(DEBUG) cout << "bits for word index: " << wordOffsetBits << " bits" << endl;
	if(DEBUG) cout << "bits for tag: " << tagBits << " bits" << endl;

	// construct array of sets
	sets = new Set [numSets];
	for(int i = 0; i < numSets; ++i)
		(sets + i) = &Set(blockSize);
	if(DEBUG) cout << "set[0]: " << sets[0].numWords << " words" << endl;

	// initialize dirty bits
	dirty = new bool [capacity];
	valid = new bool [capacity];
	for(int i = 0; i < capacity; ++i){
		valid [i] = dirty[i] = 0;
	}
	memory = new (nothrow) int [capacity];	// words
	if (memory==0){
		cout << "Failed to allocate memory!\n";
	}
     
     for(int set = 0; set < capacity; ++set){
		memory[set] = set;
     }
}

//*** destructor
CacheMemory::~CacheMemory(){
    if(memory != NULL){
		delete [] memory;
    } 
}

void CacheMemory::set_content(int location, int value){
	++writes;
}

int CacheMemory::read(int address){
	++reads;
	//if(DEBUG) cout << "SHAZAM!" << endl;
	return memory[0];
	
}

void CacheMemory::reset_content(void){
	//More code
}

void CacheMemory::print_contents(){
/*	cout << "STATISTICS:" << endl;
	cout << "Misses:" << endl;
	cout << "Total: " << misses;
	cout << " DataReads: " << reads;
	cout << " DataWrites: " << writes << endl;
	cout << "Miss rate: " << endl;
	cout << "Total: " << misses;
	cout << " DataReads: " << reads;
	cout << " DataWrites: " << writes << endl;
	cout << "Number of Dirty Blocks Evicted From the Cache: " << evicted << endl;
*/	

	cout << "CACHE MEMORY:" << endl;
	//assert -1 < format < 3
	cout << "Set    V   Tag   Dirty   Word" << endl;
	int i = 0;
	int to = capacity-1;
	while(i <= to){
//		cout << 9 << valid[i] << 0 << dirty[i] << " ";
		cout << setw(8) << setfill('0') << hex << i;		//print address first
		//print words
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
	/*
Set   V    Tag    Dirty    Word0      Word1      Word2      Word3      Word4      Word5      Word6      Word7   
0     1   00003fe8    0    003fe800   003fe801   003fe802   003fe803   003fe804   003fe805   003fe806   003fe807   
0     1   00003f80    0    003f8000   003f8001   003f8002   003f8003   66666666   003f8005   003f8006   003f8007  
*/	
}

Set::Set(){
	//if(DEBUG) cout << "Set default constructed" << endl;
	
}

Set::Set(int numWords){
	numWords = numWords;
	cacheLine = new int [numWords];
	//if(DEBUG) cout << "set properly constructed." << endl;
	
}

void Set::print(){
	cout << "Set:" << endl;
	for(int i = 0; i < numWords; ++i)
		cout << cacheLine[i] << endl;
	
	
}	
