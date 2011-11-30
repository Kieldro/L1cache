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
	hits = misses = writes = reads = evicted = 0;
	numSets = capacity / blockSize / associativity;
	
	//assert(divides nicely);
	if(DEBUG) cout << "cache capacity: " << capacity << " words" << endl;
	if(DEBUG) cout << "block size: " << blockSize << " words" << endl;
	if(DEBUG) cout << "number of sets: " << numSets << " sets" << endl;
	setBits = (float)log(numSets)/log(2);
	wordOffsetBits = (float)log(blockSize)/log(2);
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
	++reads;
	
	
	parseAddress(address, wordIdx, set, tag);		// parameters passed by reference
	data = sets[set].read(tag, wordIdx, found);	

	if(found)
	{
		cout << "Found " << address << " in set " << set << ", word " << wordIdx << endl;
		data = sets[set].read(tag, wordIdx, found);		// found passed by ref< wordIdx << endl;
		++hits;
	}
	else{		// not found in cache
		data = mem->read(address);
		if(sets[set].line[sets[set].getLRU()].dirty)	// block to be replaced is dirty; write old block to memory, then replace with new block
		{
			int start = address - (address % blockSize);		// align
			for(int i = 0; i < blockSize; i++)
			{
				mem->write(start + i, sets[set].line[sets[set].getLRU()].word[i]);
			}
			evicted++;
		}
		
		int start = address - (address % blockSize);		// start at first word in block
		for(int i = 0; i < blockSize; i++)
		{
			// i never exceeds mem.capacity because both are mulitples of blockSize
			sets[set].line[sets[set].getLRU()].word[i] = mem->read(start + i);
		}
		sets[set].line[sets[set].getLRU()].tag = tag;
		sets[set].line[sets[set].getLRU()].valid = true;
		sets[set].line[sets[set].getLRU()].dirty = false;
		sets[set].updateLRU();

		++misses;
	}
	
	if(DEBUG) sets[set].print(set);
	
	return data;
}

void CacheMemory::write (unsigned address, int data){
	unsigned wordIdx;
	unsigned set;
	unsigned tag;
	bool found = false;
	
	parseAddress(address, wordIdx, set, tag);		// parameters passed by reference
	cout << "TAG: " << tag << " and the four are ";
	sets[set].write(data, tag, wordIdx, found);
	cout << " and found: " << found << endl;

	if(!found)
		{
			if(sets[set].line[sets[set].getLRU()].dirty)
			{
				int start = address - (address % blockSize);		// align
				for(int i = 0; i < blockSize; i++)
				{
					mem->write(start + i, sets[set].line[sets[set].getLRU()].word[i]);
				}
				++evicted;
			}
			int start = address - (address % blockSize);
			for(int i = 0; i < blockSize; i++)
			{
				sets[set].line[sets[set].getLRU()].word[i] = mem->read(start + i);
			}
			sets[set].line[sets[set].getLRU()].tag = tag;
			sets[set].line[sets[set].getLRU()].valid = true;
			sets[set].line[sets[set].getLRU()].word[address % blockSize] = data; // modify it with the write
			sets[set].line[sets[set].getLRU()].dirty = true; // then mark it as dirty
			sets[set].updateLRU();
			// ++misses;??????
			
		}
	else
		 {
			for(int i = 0; i < associativity; ++i)
		   {
				if(sets[set].line[i].tag == tag){
					sets[set].line[i].word[address%blockSize] = data;
					sets[set].line[i].dirty = true;
					break;
				}
			}
		 }
		//sets[set].line[] = data;		// write in whole block??
		;
	
	if(DEBUG) sets[set].print(set);
	++writes;
}

// parse out the tag, set and word offset from the address
void CacheMemory::parseAddress (const unsigned address, unsigned &wordIdx, unsigned &set, unsigned &tag){
	wordIdx = address & wMask;
	set = (address & sMask) >> wordOffsetBits;
	// There is some weird casting behavior where:
	// wordOffsetBits = (float)log(blockSize)/log(2);
	// wordOffsetBits = log(blockSize)/log(2);
	
	tag = (address & tMask) >> (32 - tagBits);
	// if(DEBUG) cout << "wordIdx: " << wordIdx << "" << endl;
	// if(DEBUG) cout << "set: " << set << "" << endl;
	// if(DEBUG) cout << "tag: 0x" << hex << tag << "" << endl;
}

void CacheMemory::print(){
	cout << "STATISTICS:" << dec << endl;
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
	
	for(int nSets = 0; nSets < numSets; nSets++)
		sets[nSets].print(nSets);
	
	//print mem
	mem->print();
	
	/*
	Set   V    Tag    Dirty    Word0      Word1      Word2      Word3      Word4      Word5      Word6      Word7   
	0     1   00003fe8    0    003fe800   003fe801   003fe802   003fe803   003fe804   003fe805   003fe806   003fe807   
	0     1   00003f80    0    003f8000   003f8001   003f8002   003f8003   66666666   003f8005   003f8006   003f8007  
	*/
}

// run through entire cache and write all the dirty blocks to main mem (non-evicted)
void CacheMemory::writeDirtyBlocks (){
	unsigned address;
	unsigned temp;
	
	for(int s = 0; s < numSets; ++s)
		for(int l = 0; l < associativity; ++l)		// check all cacheLines in set
			if(sets[s].line[l].dirty == DIRTY){
				// splice address 
				address = 0x00;		// ZEXT
				temp = 0x00;
				address = (address & sets[s].line[l].tag) << (32 - tagBits);
				temp = (s & temp) << wordOffsetBits;
				address |= temp;
				for(int w = 0; w < blockSize; ++w)		//write block
					mem->write(address + w, sets[s].line[l].word[w]);
		}
	
}

// Set methods
Set::Set() {
	CacheLine::blockSize = blockSize;
	line = new CacheLine [associativity];		// CacheLines per set (associativity)
	LRU = 0;
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
	   {
		if(line[i].tag == tag && line[i].valid){
			found = true;
			line[i].word[wordIdx] = data;		// requested word found
			return;
		}
		}
	
	found = false;		// not found in cache
}

void Set::print(int nSets){
	for(int blocksPerSet = 0; blocksPerSet < associativity; ++blocksPerSet){
		cout << hex << nSets << "     ";
		line[blocksPerSet].print();
	}
}

void Set::updateLRU()
{
	LRU = (LRU + 1) % associativity;
}

int Set::getLRU()
{
	return LRU;
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
	cout << valid << "   " << setw(8) << setfill('0') << hex << tag << "    " << dirty << " ";
	// print words
	for(int wordsPerBlock = 0; wordsPerBlock < blockSize; ++wordsPerBlock)
		cout << "   " << setw(8) << setfill('0') << word[wordsPerBlock];
	
	cout << endl;
}
