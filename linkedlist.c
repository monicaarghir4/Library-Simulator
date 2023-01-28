// Copyright 2021-2022 Arghir Monica-Andreea

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "tools.h"

// creating a list by allocating memory and setting its data size
linked_list_t*
ll_create(unsigned int data_size)
{
	linked_list_t* ll = calloc(1, sizeof(*ll));
	DIE(!ll, "calloc list");

	ll->data_size = data_size;

	return ll;
}

// getting a certain node from a list
ll_node_t*
get_nth_node(linked_list_t* list, unsigned int n)
{
	ll_node_t* node = list->head;

	n = MIN(n, list->size - 1);

	for (unsigned int i = 0; i < n; ++i)
		node = node->next;

	return node;
}

// creating a node for a list with given data
ll_node_t*
create_node(void* new_data, unsigned int data_size)
{
	ll_node_t* node = calloc(1, sizeof(*node));
	DIE(!node, "calloc node");

	node->data = malloc(data_size);
	DIE(!node->data, "malloc data");

	memcpy(node->data, new_data, data_size);

	return node;
}

// adding a node to a list on a certain position n
void
ll_add_nth_node(linked_list_t* list, unsigned int n, void* new_data)
{
	ll_node_t *new_node, *prev_node;

	if (!list) {
		return;
    }

	new_node = create_node(new_data, list->data_size);

	if (!n || !list->size) {
		new_node->next = list->head;
		list->head = new_node;
	} else {
		prev_node = get_nth_node(list, n - 1);
		new_node->next = prev_node->next;
		prev_node->next = new_node;
	}

	list->size++;
}

// removing a certain node from a list and returning it
ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
	ll_node_t *prev_node, *removed_node;

	if (!list || !list->size)
		return NULL;

	if (!n) {
		removed_node = list->head;
		list->head = removed_node->next;
		removed_node->next = NULL;
	} else {
		prev_node = get_nth_node(list, n - 1);
		removed_node = prev_node->next;
		prev_node->next = removed_node->next;
		removed_node->next = NULL;
	}

	list->size--;

	return removed_node;
}

// eliberating the memory used for a list
void
ll_free(linked_list_t** list)
{
	ll_node_t* node;

	if (!list || !*list)
		return;

	while ((*list)->size) {
		node = ll_remove_nth_node(*list, 0);
		free(node->data);
		free(node);
	}

	free(*list);
	*list = NULL;
}
