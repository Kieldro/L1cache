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
int CacheLine::blockSize = 0;

// CacheMemory method definitions
CacheMemory::CacheMemory(int assoc, int bSize, int cap){
	if(DEBUG) cout << "CONSTRUCTING CacheMemory obj: " << this << endl;
	this->capacity = cap * pow(2, 10) / 4;		// convert KiB to words
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
	wMask = ( 1 << wordOffsetBits ) - 1;
	sMask = (( 1 << setBits ) - 1) << wordOffsetBits;
	tMask = (( 1 << tagBits ) - 1) << (32 - tagBits);
		
	if(DEBUG) cout << "word mask: 0x" << hex << wMask << "" << endl;
	if(DEBUG) cout << "set mask: 0x" << sMask << "" << endl;
	if(DEBUG) cout << "tag mask: 0x" << tMask << "" << endl;

	// construct array of sets
	Set::associativity = associativity;
	Set::blockSize = blockSize;
	sets = new Set [numSets];
	
	if(DEBUG) cout << "set[0]: " << sets[0].associativity << " cacheLines" << endl;
	
}

//*** destructor
CacheMemory::~CacheMemory(){
    if(sets != NULL){
		delete [] sets;
    }
}

int CacheMemory::read (unsigned address){
	unsigned wordIdx;
	unsigned set;
	unsigned tag;
	bool found = false;
	int data = -1;
	
	parseAddress(address, wordIdx, set, tag);		// parameters passed by reference

	data = sets[set].read(tag, wordIdx, found);		// found passed by ref
	if(found)
		++hits;
	else{
		data = mem->read(address);
		//CacheMemory.store(data) according to LRU
		++misses;
	}
	++reads;
	
	return data;
}

void CacheMemory::write (unsigned address, int data){
	unsigned wordIdx;
	unsigned set;
	unsigned tag;
	bool found = false;
	
	parseAddress(address, wordIdx, set, tag);		// parameters passed by reference
	
	sets[set].write(data, tag, wordIdx, found);
	
	if(!found)
		//sets[set].line[] = data;		// write in whole block??
		;
	
	++writes;
}

// parse out the tag, set and word offset from the address
void CacheMemory::parseAddress (const unsigned address, unsigned &wordIdx, unsigned &set, unsigned &tag){
	wordIdx = address & wMask;
	set = (address & sMask) >> (int)wordOffsetBits;
	tag = (address & tMask) >> (32 - tagBits);
	/*
	if(DEBUG) cout << "wordIdx: " << wordIdx << "" << endl;
	if(DEBUG) cout << "set: " << set << "" << endl;
	if(DEBUG) cout << "tag: 0x" << hex << tag << "" << endl;
	*/
}

void CacheMemory::print(){
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
	
	cout << "CACHE MEMORY:" << endl;
	//assert -1 < format < 3
	cout << "Set   V    Tag    Dirty    ";
	for(int i=0; i<blockSize; ++i) cout << "Word" << i << "      ";
	
	cout << endl;
	
	int numSets = capacity / blockSize / associativity;
	for(int nSets = 0; nSets < numSets; nSets++)
		sets[nSets].print(nSets);
	
	//print mem
	//mem->print();
	/*
	Set   V    Tag    Dirty    Word0      Word1      Word2      Word3      Word4      Word5      Word6      Word7   
	0     1   00003fe8    0    003fe800   003fe801   003fe802   003fe803   003fe804   003fe805   003fe806   003fe807   
	0     1   00003f80    0    003f8000   003f8001   003f8002   003f8003   66666666   003f8005   003f8006   003f8007  
	*/
}

// Set methods
Set::Set(){
	CacheLine::blockSize = blockSize;
	line = new CacheLine [associativity];		// CacheLines per set (associativity)
}

int Set::read(unsigned tag, unsigned wordIdx, bool &found){
	for(int i = 0; i < associativity; ++i)
		if(line[i].tag == tag && line[i].valid){
			found = true;
			return line[i].word[wordIdx];		// requested word found
		}
	
	found = false;
	return 0;		// word not found
}

void Set::write(int data, unsigned tag, unsigned wordIdx, bool &found){
	for(int i = 0; i < associativity; ++i)
		if(line[i].tag == tag && line[i].valid){
			found = true;
			line[i].word[wordIdx] = data;		// requested word found
		}
	
	found = false;		// not found in cache
}

void Set::print(int nSets){
	for(int blocksPerSet = 0; blocksPerSet < associativity; ++blocksPerSet){
		cout << nSets << "     ";
		line[blocksPerSet].print();
	}
}

//CacheLine methods
CacheLine::CacheLine(){
	valid = false;
	tag = 0;
	
	word = new int [blockSize];
	for(int i = 0; i < blockSize; ++i)
		word[i] = 0;
}

void CacheLine::print(){
	// valid AND dirty??
	cout << valid << "   " << setw(8) << setfill('0') << hex << tag << "    " << valid << " ";
	// print words
	for(int wordsPerBlock = 0; wordsPerBlock < blockSize; ++wordsPerBlock)
		cout << "   " << setw(8) << setfill('0') << word[wordsPerBlock];
	
	cout << endl;
}