#include "block.h"

block_t *create_block(void *start, uint size)
{
	block_t *b = (block_t *)malloc(sizeof(block_t));
	b->start = start;
	b->size = size;
	b->data = malloc(size);
	return b;
}

void free_block(void *b)
{
	block_t *block = (block_t *)b;
	free(block->data);
	free(block);
}

void print_block_short(void *b)
{
	block_t *block = b;
	printf(" %p", block->start);
}

void print_block_long(void *b)
{
	block_t *block = b;
	printf(" (%p - %u)", block->start, block->size);
}
