#include <iostream.h>
#include <stdio.h>

#define CACHE_READ 0
#define CACHE_WRITE 1

using namespace std;

int main(void)
{

	int i;
	int read_write;
	int address;
	unsigned int data;

	int memory[100];

	//initialise memory
	for(i=0; i< 100; i++)
	{
	  memory[i] = i;
	}

	// repeat till we reach the end of the input	
	while(!feof(stdin))
	{
	  	//read in whether to read or write to the cache
		cin >> dec >> read_write;

		// check again if we have reached the end
		// as this flag is set only after a 'cin'
		if(feof(stdin)) return 1;

		cin >> hex >> address;

		//if it is a cache write the we have to read the data
		if(read_write == CACHE_WRITE)
		{
		  	cin >> hex >> data;
			memory[address] = data;

			//output the new contents
			cout << hex << "memory[" << address << "] = " << dec << memory[address] << endl;
		}
		else
		{
			//output the contents
			cout << hex << "memory[" << address << "] = " << dec << memory[address] << endl;
		}

	}

}
