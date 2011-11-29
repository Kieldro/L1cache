#include <iostream>
#include <iomanip>
#include <new>
#include <math.h>

//*** the .cc file of a class should include its .h 
//T I B
//B = word index within block, 
#include "cache_memory.h"

using namespace std;

int Set::associativity = 0;
int Set::blockSize = 0;

// CacheMemory method definitions
CacheMemory::CacheMemory(int assoc, int bSize, int cap){
	capacity = cap * pow(2, 10) / 4;		// convert KiB to words
	mem = new MainMemory;		// fixed segfault
	associativity = assoc;
	blockSize = bSize / 4;		// convert from bytes to words
	//int totalBlocks = capacity;
	hits = misses = writes = reads = evicted = 0;
	
	const int numSets = capacity / blockSize / associativity;
	//assert(divides nicely);
	if(DEBUG) cout << "cache capacity: " << capacity << " words" << endl;
	if(DEBUG) cout << "block size: " << blockSize << " words" << endl;
	if(DEBUG) cout << "number of sets: " << numSets << " sets" << endl;
	setBits = log(numSets)/log(2);
	wordOffsetBits = log(blockSize)/log(2);
	tagBits = 32 - setBits - wordOffsetBits;
	if(DEBUG) cout << "bits for set index: " << setBits << " bits" << endl;
	if(DEBUG) cout << "bits for word index: " << wordOffsetBits << " bits" << endl;
	if(DEBUG) cout << "bits for tag: " << tagBits << " bits" << endl;

	// build masks
	wMask = ( 1 << (int)wordOffsetBits ) - 1;
	sMask = (( 1 << (int)setBits ) - 1) << (int)wordOffsetBits;
	tMask = (( 1 << tagBits ) - 1) << (32 - tagBits);
	
	if(DEBUG) cout << "word mask: 0x" << hex << wMask << "" << endl;
	if(DEBUG) cout << "set mask: 0x" << sMask << "" << endl;
	if(DEBUG) cout << "tag mask: 0x" << tMask << "" << endl;

	// construct array of sets
	Set::associativity = associativity;
	Set::blockSize = blockSize;
	sets = new Set [numSets];
	
	//int Set::blockSize = blockSize;
	
	if(DEBUG) cout << "set[0]: " << sets[0].associativity << " cacheLines" << endl;
	if(DEBUG) cout << "set[0].blockSize: " << sets[0].blockSize << " words" << endl;
}

//*** destructor
CacheMemory::~CacheMemory(){
    if(sets != NULL){
		delete [] sets;
    }
}

void CacheMemory::write (unsigned address, int data){
	unsigned wordIdx;
	unsigned set;
	unsigned tag;
	
	parseAddress(address, wordIdx, set, tag);		// parameters passed by reference
	
	//sets.write();

	++writes;
}

int CacheMemory::read (unsigned address){
	unsigned wordIdx;
	unsigned set;
	unsigned tag;
	int data = -1;
	
	parseAddress(address, wordIdx, set, tag);		// parameters passed by reference

	//data = sets[set].read(tag, wordIdx);
	++hits;
	/*if(!inCache){
		data = mem.read(address);
		//Store(data) according to LRU
		++misses;
	}
	*/
	++reads;
	return data;
}

// parse out the tag, set and word offset from the address
void CacheMemory::parseAddress (const unsigned address, unsigned &wordIdx, unsigned &set, unsigned &tag){
	wordIdx = address & wMask;
	set = (address & sMask) >> (int)wordOffsetBits;
	tag = (address & tMask) >> (32 - tagBits);
	
	if(DEBUG) cout << "wordIdx: " << wordIdx << "" << endl;
	if(DEBUG) cout << "set: " << set << "" << endl;
	if(DEBUG) cout << "tag: 0x" << hex << tag << "" << endl;
}

void CacheMemory::print(){
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
			 cout << mem->read(i);
		}
		cout  << endl;          
	}
	/*
Set   V    Tag    Dirty    Word0      Word1      Word2      Word3      Word4      Word5      Word6      Word7   
0     1   00003fe8    0    003fe800   003fe801   003fe802   003fe803   003fe804   003fe805   003fe806   003fe807   
0     1   00003f80    0    003f8000   003f8001   003f8002   003f8003   66666666   003f8005   003f8006   003f8007  
*/	
}

// Set methods
Set::Set(){
	line = new cacheLine [associativity];		// cacheLines per set (associativity)
	for(int i = 0; i < associativity; ++i)		// initialize each cacheLine
		line[i].word = new int [blockSize];
}

int Set::read(unsigned tag, unsigned wordIdx){
	for(int i = 0; i < associativity; ++i)
		if(line[i].tag == tag && line[i].valid && !line[i].dirty)		//dirty??
			return line[i].word[wordIdx];		// requested word found
	
	return 0;		// word not found
}

void Set::print(){
	cout << "Set:" << endl;
	for(int i = 0; i < associativity; ++i){
		for(int j = 0; j < blockSize; ++j)
			cout << line[i].word[j] << " ";
		cout << endl;
	}
}	
