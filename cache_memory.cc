#include <iostream>
#include <iomanip>
#include <new>
#include <math.h>

//*** the .cc file of a class should include its .h 
//T I B
//B = word index within block, 
#include "cache_memory.h"

using namespace std;

int CacheMemory::wordOffsetBits = -1;
int CacheMemory::tagBits = -1;
int CacheMemory::setBits = -1;
int Set::associativity = -1;
int Set::blockSize = -1;
int CacheLine::blockSize = -1;

// CacheMemory method definitions
CacheMemory::CacheMemory(int assoc, int bSize, int cap){
	if(DEBUG) cout << "CONSTRUCTING CacheMemory obj: " << this << endl;
	this->capacity = cap * pow(2, 10) / 4;		// convert KiB to words
	mem = new MainMemory;		// fixed segfault
	associativity = assoc;
	blockSize = bSize / 4;		// convert from bytes to words
	hits = misses = writes = reads = evicted = totalReads = totalWrites = 0;
	numSets = capacity / blockSize / associativity;
	if(numSets == 0)
		numSets = 1;	// minimum number of sets
	
	//assert(divides nicely);
	if(DEBUG) cout << "cache capacity: " << capacity << " words" << endl;
	if(DEBUG) cout << "block size: " << blockSize << " words" << endl;
	if(DEBUG) cout << "associativity: " << associativity << " words" << endl;
	if(DEBUG) cout << "number of sets: " << numSets << " sets" << endl;
	setBits = (float)log2(numSets);
	wordOffsetBits = (float)log2(blockSize);
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
	++totalReads;
	
	parseAddress(address, wordIdx, set, tag);		// parameters passed by reference
	data = sets[set].read(tag, wordIdx, found);		// found passed by ref

	if(found){
		//data = sets[set].read(tag, wordIdx, found);
		++hits;
	}else{		// not found in cache
		++reads;		// from memory
		data = mem->read(address);
		
		// 
		CacheLine *LRU_line = &sets[set].line[sets[set].LRU];	// LRUed line in the set
		if(LRU_line->dirty){			// block to be replaced is dirty
			// write old block to memory
			int start = spliceAddress(set, LRU_line->tag);
			for(int i = 0; i < blockSize; i++)
				mem->write(start + i, LRU_line->word[i]);
			++evicted;
		}
		
		// write in new line
		int start = address - (address % blockSize);		// start at first word in block
		for(int i = 0; i < blockSize; i++)		// write block
			// i never exceeds mem.capacity because both are mulitples of blockSize
			LRU_line->word[i] = mem->read(start + i);
		
		sets[set].line[sets[set].LRU].tag = tag;
		sets[set].line[sets[set].LRU].valid = true;
		sets[set].line[sets[set].LRU].dirty = false;
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
	++totalWrites;

	parseAddress(address, wordIdx, set, tag);		// parameters passed by reference
	sets[set].write(data, tag, wordIdx, found);

	if(!found){
			++writes;
			if(sets[set].line[sets[set].LRU].dirty){
				int start = spliceAddress(set, sets[set].line[sets[set].LRU].tag);
				for(int i = 0; i < blockSize; i++)
					mem->write(start + i, sets[set].line[sets[set].LRU].word[i]);
				
				++evicted;
			}
			int start = address - (address % blockSize);
			for(int i = 0; i < blockSize; i++)
				sets[set].line[sets[set].LRU].word[i] = mem->read(start + i);
			
			sets[set].line[sets[set].LRU].tag = tag;
			sets[set].line[sets[set].LRU].valid = true;
			sets[set].line[sets[set].LRU].word[address % blockSize] = data; // modify it with the write
			sets[set].line[sets[set].LRU].dirty = true; // then mark it as dirty
			sets[set].updateLRU();
	}else{
		for(int i = 0; i < associativity; ++i){
			if(sets[set].line[i].tag == tag){
				sets[set].line[i].word[address%blockSize] = data;
				sets[set].line[i].dirty = true;
				break;
			}
		}
	}
	
	if(DEBUG) sets[set].print(set);
}

// parse out the tag, set and word offset fields from the address into variables
void CacheMemory::parseAddress (const unsigned address, unsigned &wordIdx, unsigned &set, unsigned &tag){
	wordIdx = address & wMask;
	set = (address & sMask) >> wordOffsetBits;
	// There is some weird casting behavior where these 2 statements aren't equal:
	// wordOffsetBits = (float)log(blockSize)/log(2);
	// wordOffsetBits = log(blockSize)/log(2);
	
	tag = (address & tMask) >> (32 - tagBits);
	// if(DEBUG) cout << "wordIdx: " << wordIdx << "" << endl;
	// if(DEBUG) cout << "set: " << set << "" << endl;
	// if(DEBUG) cout << "tag: 0x" << hex << tag << "" << endl;
}

// prints contents of cache then mem
void CacheMemory::print(){
	cout << "STATISTICS:" << dec << endl;
	cout << "Misses:" << endl;
	cout << "Total: " << writes + reads;
	cout << " DataReads: " << reads;
	cout << " DataWrites: " << writes << endl;
	cout << "Miss rate: " << endl;
	//special cases
	cout << "Total: ";
	if(totalReads + totalWrites == 0)
		cout << 0;
	else
		cout << float(reads + writes) / float(totalReads + totalWrites);
	
	cout << " DataReads: ";
	if(totalReads == 0)
		cout << 0;
	else
		cout << float(reads) / float(totalReads);
	
	cout << " DataWrites: ";
	if(totalWrites == 0)
		cout << 0;
	else
		cout << float(writes) / float(totalWrites);
	
	cout << endl << "Number of Dirty Blocks Evicted From the Cache: " << evicted << endl << endl;
	
	cout << "CACHE CONTENTS" << endl;
	//assert -1 < format < 3
	cout << "Set   V    Tag    Dirty    ";
	for(int i=0; i<blockSize; ++i) cout << "Word" << i << "      ";
	
	cout << endl;
	
	for(int set = 0; set < numSets; set++)
		sets[set].print(set);
	cout << endl;
	
	//print mem
	mem->print();
}

// run through entire cache and write all the dirty blocks to main mem (non-evicted)
void CacheMemory::writeDirtyBlocks (){
	unsigned address = -1;
	
	for(int s = 0; s < numSets; ++s)
		for(int l = 0; l < associativity; ++l)		// check all cacheLines in set
			if(sets[s].line[l].dirty == DIRTY){
				address = spliceAddress(s, sets[s].line[l].tag);
				for(int w = 0; w < blockSize; ++w)		//write block
					mem->write(address + w, sets[s].line[l].word[w]);
			}
}

// reconstructs address of a block from its tag and set number
unsigned CacheMemory::spliceAddress (unsigned set, unsigned tag){	
	unsigned address = 0x00;		// ZEXT
	unsigned temp = 0x00;
	
	address = (address | tag) << (32 - tagBits);
	temp = (set | temp) << wordOffsetBits;
	address |= temp;
	
	return address;	
}

// Set methods
Set::Set() {
	CacheLine::blockSize = blockSize;
	line = new CacheLine [associativity];		// CacheLines per set (associativity)
	LRU = 0;
}

// helper for CacheMemory.read()
int Set::read(unsigned tag, unsigned wordIdx, bool &found){
	for(int i = 0; i < associativity; ++i)
		if(line[i].tag == tag && line[i].valid){
			found = true;
			return line[i].word[wordIdx];		// requested word found
		}
	found = false;
	return 0;		// word not found
}

// helper for CacheMemory.write()
void Set::write(int data, unsigned tag, unsigned wordIdx, bool &found){
	for(int i = 0; i < associativity; ++i)
		if(line[i].tag == tag && line[i].valid){
			found = true;
			line[i].word[wordIdx] = data;		// requested word found
			return;
		}
	found = false;		// not found in cache
}

// helper for CacheMemory.print()
void Set::print(int set){
	for(int l = 0; l < associativity; ++l){
		cout << hex << set << "     ";
		line[l].print();
	}
}

void Set::updateLRU(){
	LRU = (LRU + 1) % associativity;
}

//CacheLine methods
CacheLine::CacheLine(){
	valid = false;
	tag = 0;
	
	word = new int [blockSize];
	for(int i = 0; i < blockSize; ++i)
		word[i] = 0;
}

// helper for Set.print()
void CacheLine::print(){
	cout << valid << "   " << setw(8) << setfill('0') << hex << tag << "    " << dirty << " ";
	// print words
	for(int i = 0; i < blockSize; ++i)
		cout << "   " << setw(8) << setfill('0') << word[i];
	
	cout << endl;
}
