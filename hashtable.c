// Copyright 2021-2022 Arghir Monica-Andreea

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "tools.h"
#include "linkedlist.h"

// function that compares two strings
int
compare_function_strings(void *a, void *b)
{
	char *string_a = (char *)a;
	char *string_b = (char *)b;

	return strcmp(string_a, string_b);
}

// function that calculates the hash inspired from the internet
unsigned int
hash_function_string(void *a)
{
	unsigned char *a1 = (unsigned char*) a;
	__int16_t hash = 5381;
	int c;

	while ((c = *a1++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
}

// function that allocates memory and fills the data for a hashtable
hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*))
{
	if (!hash_function || !compare_function) {
		return NULL;
	}

	hashtable_t* ht = malloc(sizeof(hashtable_t));
	DIE(!ht, "Error");

	ht->size = 0;
	ht->hmax = hmax;
	ht->hash_function = hash_function;
	ht->compare_function = compare_function;

	// allocating memory for the buckets that will contain linked-lists
	ht->buckets = malloc(ht->hmax * sizeof(linked_list_t *));
	DIE(!ht->buckets, "Error");

	for (unsigned int i = 0; i < ht->hmax; ++i) {
		ht->buckets[i] = ll_create(sizeof(info));
	}

	return ht;
}

// function that returns 1 if a hashtabel already has the given key
// and 0 otherwise
int
ht_has_key(hashtable_t *ht, void *key)
{
	if (!ht || !key) {
		return -1;
	}

	// calculating the hash for knowing which bucket to look at
	int hash_index = ht->hash_function(key) % ht->hmax;
	ll_node_t* node = ht->buckets[hash_index]->head;

	while (node != NULL) {
		info* data_info = (info *)node->data;
		// if it finds the key it returns 1
		if (!ht->compare_function(data_info->key, key)) {
			return 1;
		}
		node = node->next;
	}

	return 0;
}

// function that returns the value of a specific key
void *
ht_get(hashtable_t *ht, void *key)
{
	if (!ht || !key || ht_has_key(ht, key) != 1) {
		return NULL;
	}

	int hash_index = ht->hash_function(key) % ht->hmax;
	ll_node_t* node = ht->buckets[hash_index]->head;

	while (node != NULL) {
		info* data_info = (info *)node->data;
		if (!ht->compare_function(data_info->key, key)) {
			return data_info->value;
		}
		node = node->next;
	}

	return NULL;
}

// functions that puts an pair key-value in a hashtable or
// updates the key to a new value
void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size)
{
	if (!ht || !key || !value) {
		return;
	}

	// increasing the size
	ht->size++;

	int hash_index = ht->hash_function(key) % ht->hmax;

	// if the hashtable already has the key we update its value
	if (ht_has_key(ht, key) == 1) {
		ll_node_t* node = ht->buckets[hash_index]->head;
		while (node != NULL) {
			info* data_info = node->data;

			if (!ht->compare_function(data_info->key, key)) {
				// eliberating the value from before
				free(data_info->value);

				data_info->value = malloc(value_size);
				DIE(!data_info, "Error");

				memcpy(data_info->value, value, value_size);
				return;
			}

			node = node->next;
		}
	}

	// allocating memory for each section and copying the bytes
	info* data_info = malloc(sizeof(info));
	DIE(!data_info, "Error");

	data_info->key = malloc(key_size);
	DIE(!data_info->key, "Error");
	data_info->value = malloc(value_size);
	DIE(!data_info->value, "Error");

	memcpy(data_info->key, key, key_size);
	memcpy(data_info->value, value, value_size);

	// adding the data to the list
	ll_add_nth_node(ht->buckets[hash_index],
		ht->buckets[hash_index]->size, data_info);

	free(data_info);
}

// function that deletes a pair of value-key from the hashtabel
void
ht_remove_entry(hashtable_t *ht, void *key)
{
	if (!ht || !key || ht_has_key(ht, key) != 1) {
		return;
	}

	int hash_index = ht->hash_function(key) % ht->hmax;
	ll_node_t* node = ht->buckets[hash_index]->head;

	unsigned int node_nr = 0;

	while (node != NULL) {
		info* data_info = (info *) node->data;

		if (!ht->compare_function(data_info->key, key)) {
			// when it finds the entry that has to be deleted, it elibartes
			// the memory and removes the node from the list
			free(data_info->key);
			free(data_info->value);
			free(data_info);

			ll_node_t* deleted_node =
				ll_remove_nth_node(ht->buckets[hash_index], node_nr);
			free(deleted_node);

			ht->size--;
			return;
		}

		node = node->next;
		node_nr++;
	}
}

// function that eliberates the memory of a usual hashtabel
void
ht_free(hashtable_t *ht)
{
	if (!ht) {
		return;
	}

	for (unsigned int i = 0; i < ht->hmax; ++i) {
		ll_node_t* node = ht->buckets[i]->head;

		while (node != NULL) {
			info* data_info = (info *)node->data;
			free(data_info->key);
			free(data_info->value);
			node = node->next;
		}

		ll_free(&ht->buckets[i]);
	}

	free(ht->buckets);
	free(ht);
}

// function that resizes a hashtabels of definitions
void
resize_for_def(hashtable_t *defs)
{
	// changing the maximum size of the buckets
	unsigned int new_hmax = defs->hmax * 2;

	// creating a new hashtable that will be copied to the given one
	hashtable_t *new_defs = ht_create(new_hmax,
		defs->hash_function, defs->compare_function);

	// going through every definition to copy it in the new hashtable
	for (unsigned int i = 0; i < defs->hmax; i++) {
		linked_list_t *bucket = defs->buckets[i];
		ll_node_t *curr = bucket->head;

		while (curr) {
			info *data = (info *)curr->data;
			char word[MAX_STRING_SIZE];
			char def[MAX_STRING_SIZE];

			snprintf(word, MAX_STRING_SIZE, "%s", (char *)data->key);
			snprintf(def, MAX_STRING_SIZE, "%s", (char *)data->value);

			ht_put(new_defs, word, MAX_STRING_SIZE, def, MAX_STRING_SIZE);

			curr = curr->next;
		}
	}

	defs = new_defs;

	// eliberating the memory used for the auxiliary hashtable
	ht_free(new_defs);
}

// function that resizes a library
void
resize_for_lib(hashtable_t *library)
{
	unsigned int new_hmax = library->hmax * 2;

	hashtable_t *new_lib = ht_create(new_hmax,
		library->hash_function, library->compare_function);

	for (unsigned int i = 0; i < library->hmax; i++) {
		linked_list_t *bucket = library->buckets[i];
		ll_node_t *curr = bucket->head;

		while (curr) {
			info *data = (info *)curr->data;
			char book_name[MAX_STRING_SIZE];
			book_value_t *book_val = (book_value_t *)data->value;

			snprintf(book_name, MAX_STRING_SIZE, "%s", (char *)data->key);

			ht_put(new_lib, book_name, MAX_STRING_SIZE, book_val, sizeof(book_value_t));

			curr = curr->next;
		}
	}

	library = new_lib;

	ht_free(new_lib);
}

// function that resizes a hashtable of users
void
resize_for_users(hashtable_t *users)
{
	unsigned int new_hmax = users->hmax * 2;

	hashtable_t *new_users = ht_create(new_hmax,
		users->hash_function, users->compare_function);

	for (unsigned int i = 0; i < users->hmax; i++) {
		linked_list_t *bucket = users->buckets[i];
		ll_node_t *curr = bucket->head;

		while (curr) {
			info *data = (info *)curr->data;
			char user_name[MAX_STRING_SIZE];
			user_value_t *user_val = (user_value_t *)data->value;

			snprintf(user_name, MAX_STRING_SIZE, "%s", (char *)data->key);

			ht_put(new_users, user_name, MAX_STRING_SIZE,
				user_val, sizeof(user_value_t));

			curr = curr->next;
		}
	}

	users = new_users;

	ht_free(new_users);
}
