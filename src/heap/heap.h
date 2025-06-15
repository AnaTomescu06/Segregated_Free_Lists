#include <string.h>
#include "../list/list.h"
#include "../block/block.h"

#define HEAP_SUCCESS 0
#define HEAP_OUT_OF_MEMORY 1
#define HEAP_INVALID_FREE 2
#define HEAP_SEG_FAULT 3

typedef unsigned int uint;

typedef struct {
	// List of blocks
	node_t *blocks;

	// Size of each block
	uint block_size;
} heaplist_t;

typedef struct {
	// List of lists of block
	node_t *lists;

	// Number of lists
	uint list_count;

	// Total size of each list before any mallocs
	uint size;

	// Defragmentation type
	char type;

	// List of allocated blocks
	node_t *used_blocks;

	// Count of malloc operations
	uint malloc_count;

	// Count of fragmentations
	uint fragmentation_count;

	// Count of free operations
	uint free_count;
} heap_t;

// Initialize heap
heap_t *init_heap(void *start, uint list_count, uint size, char type);

// Free all memory occupied by the heap
void free_heap(heap_t *heap);

// Prints the memory dump of the heap
void dump_heap(heap_t *heap);

// Initialize a new heap list and fills it with blocks
heaplist_t *init_heap_list(void *start, uint block_size, uint block_count);

// Frees all memory occupied by the heap list
void free_heap_list(void *heap_list);

// Prints the heap list in a human readably format
void print_heap_list(void *heap_list);

// Inserts a new block in the heap
void insert_block(heap_t *heap, block_t *block);

// Allocates a block from the heap and moves it in the used_blocks list
int heap_malloc(heap_t *heap, uint alloc_size);

// Deallocates a block from used_blocks list and moves it back into the heap
int heap_free_address(heap_t *heap, void *addr);

// Writes data in an allocated block
int write_data(heap_t *heap, void *addr, char *data, uint len);

// Reads data from an allocated block
int read_data(heap_t *heap, void *addr, uint len);
