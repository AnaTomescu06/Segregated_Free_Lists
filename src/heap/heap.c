#include <stdio.h>
#include "heap.h"
#include "math.h"

heap_t *init_heap(void *start, uint list_count, uint size, char type)
{
	// Initialize heap
	heap_t *heap = (heap_t *)malloc(sizeof(heap_t));
	heap->lists = create_node(NULL);
	heap->used_blocks = create_node(NULL);
	heap->list_count = list_count;
	heap->size = size;
	heap->type = type;
	heap->malloc_count = 0;
	heap->fragmentation_count = 0;
	heap->free_count = 0;

	// Create blocks
	for (uint i = 0; i < list_count; i++) {
		uint block_size = pow(2, i + 3);
		uint block_count = size / block_size;
		heaplist_t *new_list = init_heap_list(start, block_size, block_count);
		start = (void *)((char *)start + size);
		insert_after(create_node(new_list), heap->lists->prev);
	}
	return heap;
}

heaplist_t *init_heap_list(void *start, uint block_size, uint block_count)
{
	// Create a new heap list
	heaplist_t *heap_list = (heaplist_t *)malloc(sizeof(heaplist_t));
	heap_list->blocks = create_node(NULL);
	heap_list->block_size = block_size;

	// Insert empty blocks in heap list
	for (uint i = 0; i < block_count; i++) {
		block_t *new_block = create_block(start, block_size);
		insert_after(create_node(new_block), heap_list->blocks->prev);
		start += block_size;
	}
	return heap_list;
}

void free_heap(heap_t *heap)
{
	free_list(heap->used_blocks, free_block);
	free_list(heap->lists, free_heap_list);
	free(heap);
}

void free_heap_list(void *_heap_list)
{
	heaplist_t *heap_list = (heaplist_t *)_heap_list;
	free_list(heap_list->blocks, free_block);
	free(heap_list);
}

node_t *find_block_node(heap_t *heap, uint block_size)
{
	// Find the first free block of size <block_size> in the heap
	for (node_t *i = heap->lists->next; i->data; i = i->next) {
		heaplist_t *hl = i->data;
		if (hl->block_size >= block_size && !is_empty(hl->blocks))
			return hl->blocks->next;
	}
	return NULL;
}

void insert_block(heap_t *heap, block_t *block)
{
	// Find the list where to insert the block
	node_t *target_list_node = NULL;
	heaplist_t *target_list = NULL;
	for (node_t *i = heap->lists->next; i->data; i = i->next) {
		heaplist_t *hl = i->data;
		if (hl->block_size >= block->size) {
			target_list_node = i;
			target_list = hl;
			break;
		}
	}

	if (target_list->block_size > block->size) {
		// There is no suitable list for the block, we need to create a new one
		heaplist_t *new_heap_list = (heaplist_t *)malloc(sizeof(heaplist_t));
		new_heap_list->blocks = create_node(NULL);
		new_heap_list->block_size = block->size;
		insert_after(create_node(block), new_heap_list->blocks);
		insert_after(create_node(new_heap_list), target_list_node->prev);
	} else {
		// Block fits in an existing list
		if (is_empty(target_list->blocks)) {
			// List is empty, the new block will be the first one
			insert_after(create_node(block), target_list->blocks);
		} else {
			// Find a spot for the new block in the list
			node_t *n = NULL;
			for (n = target_list->blocks->next; n->data; n = n->next) {
				block_t *b = n->data;
				if (b->start > block->start)
					break;
			}
			insert_after(create_node(block), n->prev);
		}
	}
}

int heap_malloc(heap_t *heap, uint alloc_size)
{
	// Find a block in the heap
	node_t *target_block_node = find_block_node(heap, alloc_size);
	if (!target_block_node)
		return HEAP_OUT_OF_MEMORY;
	block_t *target_block = target_block_node->data;

	// Create a new block and insert it in the used_blocks list
	block_t *used_block = create_block(target_block->start, alloc_size);
	if (is_empty(heap->used_blocks)) {
		insert_after(create_node(used_block), heap->used_blocks);
	} else {
		node_t *n = NULL;
		for (n = heap->used_blocks->next; n->data; n = n->next) {
			block_t *b = n->data;
			if (b->start > used_block->start)
				break;
		}
		insert_after(create_node(used_block), n->prev);
	}

	// Fragment the block if necessarily
	if (alloc_size != target_block->size) {
		void *new_start = used_block->start + alloc_size;
		uint new_size = target_block->size - alloc_size;
		block_t *remaining_block = create_block(new_start, new_size);

		insert_block(heap, remaining_block);
		heap->fragmentation_count++;
	}

	// Remove the used block from the heap and free the memory
	remove_node(target_block_node);
	free_node(target_block_node, free_block);
	return HEAP_SUCCESS;
}

// Returns the size of the block
uint get_block_size(void *block)
{
	return ((block_t *)block)->size;
}

// Returns the number of blocks in a heap list
uint get_heap_list_free_blocks(void *heap_list)
{
	return list_count(((heaplist_t *)heap_list)->blocks, NULL);
}

void print_heap_list(void *_heap_list)
{
	heaplist_t *heap_list = _heap_list;
	uint size = heap_list->block_size;
	uint count = list_count(heap_list->blocks, NULL);
	if (count == 0)
		return;
	printf("Blocks with %u bytes - %u free block(s) :", size, count);
	list_foreach(heap_list->blocks, print_block_short);
	printf("\n");
}

void dump_heap(heap_t *heap)
{
	uint total_memory = heap->list_count * heap->size;
	uint total_allocated_memory = list_count(heap->used_blocks, get_block_size);
	uint total_free_memory = total_memory - total_allocated_memory;
	uint free_blocks = list_count(heap->lists, get_heap_list_free_blocks);
	uint allocated_blocks = list_count(heap->used_blocks, NULL);
	printf("+++++DUMP+++++\n");
	printf("Total memory: %u bytes\n", total_memory);
	printf("Total allocated memory: %u bytes\n", total_allocated_memory);
	printf("Total free memory: %u bytes\n", total_free_memory);
	printf("Free blocks: %u\n", free_blocks);
	printf("Number of allocated blocks: %u\n", allocated_blocks);
	printf("Number of malloc calls: %u\n", heap->malloc_count);
	printf("Number of fragmentations: %u\n", heap->fragmentation_count);
	printf("Number of free calls: %u\n", heap->free_count);
	list_foreach(heap->lists, print_heap_list);
	printf("Allocated blocks :");
	list_foreach(heap->used_blocks, print_block_long);
	printf("\n");
	printf("-----DUMP-----\n");
}

int heap_free_address(heap_t *heap, void *addr)
{
	if (addr == 0x0)
		return HEAP_SUCCESS;

	// Find the block in the used blocks list
	for (node_t *i = heap->used_blocks->next; i->data; i = i->next) {
		block_t *b = (block_t *)i->data;
		if (b->start == addr) {	// Block found
			// Remove block from used blocks list
			remove_node(i);
			free(i);

			// Insert the block back in the list
			insert_block(heap, b);
			return HEAP_SUCCESS;
		}
	}
	return HEAP_INVALID_FREE;
}

/**
 * Returns the size of the continous allocated memory in the used blocks list
 * starting at the given node.
*/
uint get_continous_alloc_memory_size(node_t *node)
{
	uint size = 0;
	for (node_t *i = node; i->data; i = i->next) {
		block_t *current_block = i->data;
		size += current_block->size;

		block_t *next_block = i->next->data;
		void *target_start = current_block->start + current_block->size;
		if (!next_block || next_block->start != target_start)
			return size;
	}
	return size;
}

int write_data(heap_t *heap, void *addr, char *data, uint len)
{
	// Find the allocated block
	node_t *start = NULL;
	for (node_t *n = heap->used_blocks->next; n->data; n = n->next) {
		block_t *b = n->data;
		if (b->start == addr) {
			start = n;
			break;
		}
	}
	if (!start)
		return HEAP_SEG_FAULT;

	// Check if the data fits
	uint allocated_size = get_continous_alloc_memory_size(start);
	len = (len > strlen(data)) ? strlen(data) : len;
	if (allocated_size < len)
		return HEAP_SEG_FAULT;

	// Write the data
	while (len > 0) {
		block_t *b = start->data;
		int current_write_size = len > b->size ? b->size : len;
		strncpy(b->data, data, current_write_size);
		len -= current_write_size;
		data += current_write_size;
		start = start->next;
	}
	return HEAP_SUCCESS;
}

int read_data(heap_t *heap, void *addr, uint len)
{
	// Find allocated block
	node_t *start = NULL;
	for (node_t *n = heap->used_blocks->next; n->data; n = n->next) {
		block_t *b = n->data;
		if (b->start == addr) {
			start = n;
			break;
		}
	}
	if (!start)
		return HEAP_SEG_FAULT;

	// Check if the desired length is allocated
	uint allocated_size = get_continous_alloc_memory_size(start);
	if (allocated_size < len)
		return HEAP_SEG_FAULT;

	// Print the data
	while (len > 0) {
		block_t *b = start->data;
		int current_read_size = len > b->size ? b->size : len;
		printf("%.*s", current_read_size, b->data);
		len -= current_read_size;
		start = start->next;
	}
	printf("\n");
	return HEAP_SUCCESS;
}
