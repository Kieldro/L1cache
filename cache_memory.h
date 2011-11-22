#ifndef _CACHE_MEMORY_H
#define _CACHE_MEMORY_H

#define HEX 0
#define DEC 1
#define OCT 2

#include<iostream>

using namespace std;

class CacheMemory{
	private:
		int *memory;
		int set;
		bool *valid;
		int *tag;
		bool *dirty;
		int *data;
		int size;
		int blocksize;
		int associativity;
	public: 
		CacheMemory(int capacity);
		~CacheMemory();
		void print_contents(int from, int to, int format);
		void set_content(int location, int value);
		void reset_content(void);
};

CacheMemory::CacheMemory(int capacity){
	// initialize dirty bits
	dirty = new bool [capacity];
	for(int i = 0; i < capacity; ++i){
		valid [i] = dirty[i] = 1;
		
	}
}

void CacheMemory::set_content(int location, int value){
	//More code
}

void CacheMemory::reset_content(void){
	//More code
}

void CacheMemory::print_content(int from, int to, int format){
	//More code
	cout << "CACHE MEMORY:" << endl;
	//assert -1 < format < 3
	cout << "Set    V   Tag   Dirty   Word" << endl;
	int i = from;
	while(i <= to){
		cout << set << valid[i] << *tag << dirty[i] << setw(8) << setfill('0') << hex << i;		//print address first
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
#endif

