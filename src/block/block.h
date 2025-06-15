#include <stdlib.h>
#include <stdio.h>

typedef unsigned int uint;

typedef struct {
	// Starting address for the block
	void *start;

	// Size of block in bytes
	uint size;

	// Pointer to the data stored in the block
	char *data;
} block_t;

/**
 * Allocates memory for a new block.
 * @param start block's starting address
 * @param size block's size
 * @returns a newly created block
*/
block_t *create_block(void *start, uint size);

/**
 * Frees the memory allocated for the block
 * @param b the block
 * @returns void
*/
void free_block(void *b);

/**
 * Prints the start address of the block in the format " <start address>"
 * @param b block
 * @returns void
*/
void print_block_short(void *b);

/**
 * Prints the start address and size of block.
 * The format is " (<address> - <size>)"
 * @param b the block
 * @returns void
*/
void print_block_long(void *b);
