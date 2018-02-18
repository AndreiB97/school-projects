1. Description



	This is a small C program that simulates memory allocation. The memory is represented by an array that is allocated by the user. The user can then request a block of memory, the program must then find a suitable free area within the array, allocate it and return and index to said area. The user can also free a portion of memory that was previously allocated. Each block of memory has two sections. The first section is a management area (a 12 byte area cosisting of three integers). The first integer contains the starting index of the next block of memory or 0 if this is the last block of memory. The second integer contains the starting index of the previous block of memory or 0 if this is the first block of memory. The third integer contains the size of the block (including the management area). The second section is the data area and contains the user's data.



2. Usage



	The following commands are available:

		
	INITIALIZE SIZE

	
	This command must be used first, it is the size of the array that simulates the memory. It does not print anything

.
	
	FINALIZE

	
	This command must be used last, it free the memory allocated for the array and stops the program. It does not print anything

.
	
	DUMP

	
	Prints a map of the memory

.

	ALLOC SIZE
	Allocates a block of SIZE bytes. Prints the index of the block's data area or 0 if the allocation failed.

	FREE INDEX
	Frees the block of memory whose data area starts at INDEX.

	FILL INDEX SIZE VALUE
	Sets SIZE bytes, starting at INDEX, with VALUE. This command can overwrite management areas if used improperly.

	SHOW FREE
	Prints the amount of free areas and free bytes in the memory.

	SHOW USAGE
	Prints the amount of bytes used along, number of currently allocated blocks, the use efficiency (number of bytes used by data areas divided by the number of bytes used by the whole blocks) and fragmentation (number of free ares divided by the number of allocated blocks).

	SHOW ALLOCATIONS
	Prints all the free and occupied areas along with their size.

3. Example

	INITIALIZE 100
	ALLOC 13
	>16
	FILL 16 13 255
	DUMP
	>00000000 04 00 00 00 00 00 00 00 00 00 00 00 19 00 00 00
	>00000010 FF FF FF FF FF FF FF FF FF FF FF FF FF 00 00 00
	>00000020 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
	>00000030 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
	>00000040 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
	>00000050 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
	>00000060 00 00 00 00
	>00000064
	FREE 16
	ALLOC 50
	>16
	ALLOC 40
	>0
	ALLOC 30
	>0
	ALLOC 20
	>78
	FILL 78 20 127
	DUMP
	>00000000 04 00 00 00 42 00 00 00 00 00 00 00 3E 00 00 00
	>00000010 FF FF FF FF FF FF FF FF FF FF FF FF FF 00 00 00
	>00000020 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
	>00000030 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
	>00000040 00 00 00 00 00 00 04 00 00 00 20 00 00 00 7F 7F
	>00000050 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F
	>00000060 7F 7F 00 00
	>00000064
	FREE 16
	FREE 78
	FINALIZE