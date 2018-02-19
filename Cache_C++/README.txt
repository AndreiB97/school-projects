1. Description
	This program simulates the way a CPU's cache memory works using a hashtable. There are two possible operations: read and write. The read operation first searches in the L1 cache, if it doesn't find it then it searches in the L2 cache and if it's not there either it takes the value from the ram (represented by a file named ram.in) and saves it in L1 and L2. The write operation first searches the L1 cache, if it finds it then it updates it in L1 and copies it in L2, if it doesn't find it in L1 then it searches the L2 cache, if it finds it then it copies it in L1 and updates it in L1 and L2, if it doesn't find it then it takes it from the ram, copies it in L1 and L2 and updates it in both.

2. Usage
	There are two input files required: ram.in and operations.in. Ram.in has the following structure: separate lines, each containing two positive integers: address and value. Operations.in has separate lines with the following format: coreNumber operation address newData
	- coreNumber is either 0 or 1
	- operation is either r (read) or w (write)
	- newData is only used by the write operation
	When the program finishes it writes the cache and ram after all the modifications in cache.out and ram.out.

3. Example
	Using the following input:

	operations.in

	0 r 0
	1 w 0 124
	1 r 4
	0 w 4 1642

	ram.in

	0000000000 0004690033
	0000000004 0000097990

	The program outputs:

	cache.out

	>0 0 0 4690033 *
	>1 0 4 1642
	
	>0 0 0 124
	>1 0 4 97990 *

	>0 0 0 124
	>1 0 4 1642

	ram.out

	>0000000000 0004690033
	>0000000004 0000097990	