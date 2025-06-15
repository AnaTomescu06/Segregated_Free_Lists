#include <stdio.h>
#include <string.h>
#include "src/heap/heap.h"

int handle_destroy_heap(heap_t **heap)
{
	free_heap(*heap);
	return 1;
}

int handle_init_heap(heap_t **heap)
{
	void *addr;
	int list_count;
	int list_size;
	int t;
	fscanf(stdin, "%p %d %d %d", &addr, &list_count, &list_size, &t);
	*heap = init_heap(addr, list_count, list_size, t);
	return 0;
}

int handle_malloc(heap_t **heap)
{
	unsigned int target_size;
	fscanf(stdin, "%u", &target_size);
	int result = heap_malloc(*heap, target_size);
	if (result == HEAP_SUCCESS)
		(*heap)->malloc_count++;
	if (result == HEAP_OUT_OF_MEMORY)
		printf("Out of memory\n");
	return 0;
}

int handle_dump_memory(heap_t **heap)
{
	dump_heap(*heap);
	return 0;
}

int handle_free(heap_t **heap)
{
	void *addr;
	fscanf(stdin, "%p", &addr);
	int result = heap_free_address(*heap, addr);
	if (result == HEAP_SUCCESS)
		(*heap)->free_count++;
	else if (result == HEAP_INVALID_FREE)
		printf("Invalid free\n");
	return 0;
}

int handle_write(heap_t **heap)
{
	void *addr;
	char data[600];
	unsigned int len;
	fscanf(stdin, "%p \"%[^\"]\" %u", &addr, data, &len);
	int result = write_data(*heap, addr, data, len);
	if (result == HEAP_SEG_FAULT) {
		printf("Segmentation fault (core dumped)\n");
		dump_heap(*heap);
		free_heap(*heap);
		return 1;
	}
	return 0;
}

int handle_read(heap_t **heap)
{
	void *addr;
	unsigned int len;
	fscanf(stdin, "%p %u", &addr, &len);
	int result = read_data(*heap, addr, len);
	if (result == HEAP_SEG_FAULT) {
		printf("Segmentation fault (core dumped)\n");
		dump_heap(*heap);
		free_heap(*heap);
		return 1;
	}
	return 0;
}

int main(void)
{
	heap_t *heap = NULL;
	while (1) {
		// Read command
		char cmd[64];
		fscanf(stdin, "%s", cmd);

		int (*cmd_handler)(heap_t **) = NULL;

		if (strcmp(cmd, "DESTROY_HEAP") == 0)
			cmd_handler = handle_destroy_heap;
		else if (strcmp(cmd, "INIT_HEAP") == 0)
			cmd_handler = handle_init_heap;
		else if (strcmp(cmd, "MALLOC") == 0)
			cmd_handler = handle_malloc;
		else if (strcmp(cmd, "DUMP_MEMORY") == 0)
			cmd_handler = handle_dump_memory;
		else if (strcmp(cmd, "FREE") == 0)
			cmd_handler = handle_free;
		else if (strcmp(cmd, "WRITE") == 0)
			cmd_handler = handle_write;
		else if (strcmp(cmd, "READ") == 0)
			cmd_handler = handle_read;

		if (cmd_handler)
			if (cmd_handler(&heap))
				break;
	}
	return 0;
}
