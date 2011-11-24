#include <iostream>
#include <iomanip>
#include <new>

//*** the .cc file of a class should include its .h 
#include "cache_memory.h"

using namespace std;

CacheMemory::CacheMemory(MainMemory m, int a, int b, int c){
	capacity = c;
	mem = &m;
	associativity = a;
	blocksize = b;
	hits = misses = writes = reads = evicted = 0;
	
	// initialize dirty bits
	dirty = new bool [capacity];
	valid = new bool [capacity];
	if(DEBUG) cout << "SHAZAM!" << endl;
	for(int i = 0; i < capacity; ++i){
		valid [i] = dirty[i] = 0;
		
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

void CacheMemory::reset_content(void){
	//More code
}

void CacheMemory::print_contents(int from, int to, int format){
	cout << "STATISTICS:" << endl;
	cout << "Misses:" << endl;
	cout << "Total: " << misses;
	cout << " DataReads: " << reads;
	cout << " DataWrites: " << writes << endl;
	cout << "Miss rate: " << endl;
	cout << "Total: " << misses;
	cout << " DataReads: " << reads;
	cout << " DataWrites: " << writes << endl;
	cout << "Number of Dirty Blocks Evicted From the Cache: " << evicted << endl;
	
	//More code
	cout << "CACHE MEMORY:" << endl;
	//assert -1 < format < 3
	cout << "Set    V   Tag   Dirty   Word" << endl;
	int i = from;
	while(i <= capacity-1){
		cout << set << valid[i] << 0 << dirty[i] << setw(8) << setfill('0') << hex << i;		//print address first
		//print words
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
	/*
Set   V    Tag    Dirty    Word0      Word1      Word2      Word3      Word4      Word5      Word6      Word7   
0     1   00003fe8    0    003fe800   003fe801   003fe802   003fe803   003fe804   003fe805   003fe806   003fe807   
0     1   00003f80    0    003f8000   003f8001   003f8002   003f8003   66666666   003f8005   003f8006   003f8007  
*/	
}
