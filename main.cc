/*
Ian Buitrago
Miguel Diaz
*/

#include "main_memory.cc"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "funcs.h"

using namespace std;

void test()
{
	MainMemory mem(4000000);
	mem.print_contents(12, 24, DEC);
}

int main (int argc, char *argv[ ])
{

  //the values that we get from the 
  //command line will be put into these
  //variables. we can use them later in the program
  //like for checking if they have valid values
  int cache_capacity = 0;
  int cache_blocksize = 0;
  int cache_associativity = 0;

  if(!parseParams(argc, argv, cache_capacity, cache_blocksize, cache_associativity)) {
	exit(2);
  }
  
  // -c <capacity> with <capacity> in KB: 4, 8, 16, 32, or 64. 
  assert(cache_capacity == 4 || cache_capacity == 8 || cache_capacity == 16
         || cache_capacity == 32 || cache_capacity == 64); 
  // -b <blocksize> with <blocksize> in bytes:
  //  4, 8, 16, 32, 64, 128, 256, or 512. 
  assert(cache_blocksize == 4 || cache_blocksize == 8 || cache_blocksize == 16
         || cache_blocksize == 32 || cache_blocksize == 64 
         || cache_blocksize == 128 || cache_blocksize == 256
         || cache_blocksize == 512); 
  // -a <associativity> where <associativity> is integer size of set:
  // 1, 2, 4, 8, 16. 
  assert(cache_associativity == 1 || cache_associativity == 2
         || cache_associativity == 4 || cache_associativity == 8
         || cache_associativity == 16);
  
  cout << "Cache Capacity: " << cache_capacity << endl;
  cout << "Cache BlockSize: " << cache_blocksize << endl;
  cout << "Cache Associativity: " << cache_associativity << endl;
test();
}
