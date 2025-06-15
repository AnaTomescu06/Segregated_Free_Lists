#include <stdlib.h>

typedef unsigned int uint;

typedef struct node {
	// Pointer to the data stored in the nod
	void *data;

	// Pointer to the next node in the list
	struct node *next;

	// Pointer to the previous node in the list
	struct node *prev;
} node_t;

/**
 * Allocates memory for a new node.
 * @param data
 * @returns A newly allocated node
*/
node_t *create_node(void *data);

/**
 * Frees memory occupied by a node
 * @param node
 * @param data_free_fn function that frees the data inside the node
 * @returns void
*/
void free_node(node_t *node, void (*data_free_fn)(void *));

/**
 * Frees memory occupied by an entire list
 * @param node any node in the list
 * @param data_free_fn function that frees the data inside the nodes
 * @returns void
*/
void free_list(node_t *node, void (*data_free_fn)(void *));

/**
 * Unlinks a node from the list
 * @param node
 * @returns void
*/
void remove_node(node_t *node);

/**
 * Inserts a new node after a given node in the list
 * @param new the new node
 * @param ref a node in the list. new node will be inserted after this node
 * @returns void
*/
void insert_after(node_t *new, node_t *ref);

/**
 * Checks if the list is empty
 * @param list the first node in the list (sentinel)
 * @returns 1 if it is empty / 0 if it is not empty
*/
int is_empty(node_t *list);

/**
 * Counts elements in the list
 * A custom function can be passed in order to change the count.
 * Example: If each node in the list contains a string:
 * - list_count(list, NULL) = the length of the list
 * - list_count(list, strlen) = the sum of all string lengths
 * @param list the first node in the list (sentinel)
 * @param f a function that can be applied on the node's data
 * @returns The computed count
*/
uint list_count(node_t *list, uint (*f)(void *));

/**
 * Applies a function on each node's data
 * @param list the first node in the list (sentinel)
 * @param f a function that will be applied on the node's data
 * @returns void
*/
void list_foreach(node_t *list, void (*f)(void *));
