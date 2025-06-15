#include "list.h"

node_t *create_node(void *data)
{
	node_t *n = (node_t *)malloc(sizeof(node_t));
	n->data = data;
	n->prev = n;
	n->next = n;
	return n;
}

void free_node(node_t *node, void (*data_free_fn)(void *))
{
	if (node->data)
		(*data_free_fn)(node->data);
	free(node);
}

void free_list(node_t *node, void (*data_free_fn)(void *))
{
	node_t *start = node;
	node_t *current = node;
	node_t *next = NULL;

	while (current && next != start) {
		next = current->next;
		free_node(current, data_free_fn);
		current = next;
	}
}

void remove_node(node_t *node)
{
	node->next->prev = node->prev;
	node->prev->next = node->next;
	node->next = node;
	node->prev = node;
}

void insert_after(node_t *new, node_t *ref)
{
	ref->next->prev = new;
	new->next = ref->next;
	new->prev = ref;
	ref->next = new;
}

int is_empty(node_t *list)
{
	return list->next == list;
}

uint list_count(node_t *list, uint (*f)(void *))
{
	uint count = 0;
	for (node_t *i = list->next; i->data; i = i->next) {
		if (f)
			count += f(i->data);
		else
			count += 1;
	}
	return count;
}

void list_foreach(node_t *list, void (*f)(void *))
{
	for (node_t *i = list->next; i->data; i = i->next)
		f(i->data);
}
