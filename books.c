// Copyright 2021-2022 Arghir Monica-Andreea

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "tools.h"
#include "linkedlist.h"
#include "hashtable.h"
#include "input.h"

// function that removes a book from a library
void
rmv_book(hashtable_t *library, char book_name[])
{
	// if the book is not in the library it doesn't happen anything
	if (!ht_has_key(library, book_name)) {
		printf("%s", NOT_HERE);
		return;

	} else {
		// calculating the hash for the index of the bucket
		int hash_index = library->hash_function(book_name) % library->hmax;
		ll_node_t* node = library->buckets[hash_index]->head;

		unsigned int node_nr = 0;

		while (node != NULL) {
			info* data_info = (info *) node->data;

			if (!library->compare_function(data_info->key, book_name)) {
				// when it finds the wanted book we free its memory
				// and delete the node from the list
				free(data_info->key);
				book_value_t *book = (book_value_t *)data_info->value;
				ht_free(book->ht_def);
				free(data_info->value);
				free(data_info);

				ll_node_t* deleted_node =
					ll_remove_nth_node(library->buckets[hash_index], node_nr);
				free(deleted_node);

				library->size--;
				return;
			}
			node = node->next;
			node_nr++;
		}
	}
}

// function that adds a book to the library
void
add_book(hashtable_t *library, char book_name[], int def_number)
{
	// if the library already has the book
	// we delete it to be able to update it
	if (ht_has_key(library, book_name)) {
		rmv_book(library, book_name);
	}

	book_value_t *val = malloc(sizeof(book_value_t));
	DIE(!val, "Error");

	// creating the hashtable for definitions
	val->ht_def =
		ht_create(10, hash_function_string, compare_function_strings);

	// adding the definitions to the book
	for (int i = 0; i < def_number; i++) {
		char def_key[MAX_STRING_SIZE], def_val[MAX_STRING_SIZE];
		scanf("%s %s", def_key, def_val);

		ht_put(val->ht_def, def_key, MAX_STRING_SIZE,
			def_val, MAX_STRING_SIZE);
	}

	// if the load factor is bigger than 1 we resize the hashtable
	// for definitions
	if ((float)val->ht_def->size / (float)val->ht_def->hmax > 1.0) {
		resize_for_def(val->ht_def);
	}

	// setting the characteristics of the book
	val->reachable = 1;
	val->loans = 0;
	val->score = 0;

	// adding the book to the library
	ht_put(library, book_name, MAX_STRING_SIZE,
		val, sizeof(book_value_t));

	// verifying the load factor and resizing the library if needed
	if ((float)library->size / (float)library->hmax > 1.0) {
		resize_for_lib(library);
	}

	free(val);
}

// function that adds a new definition to a book
void
add_def(hashtable_t *library, char book_name[],
			char new_def_key[], char new_def_val[])
{
	// checking if the book is in the library
	if (!ht_has_key(library, book_name)) {
		printf("%s", NOT_HERE);
		return;

	} else {
		int hash_index = library->hash_function(book_name) % library->hmax;

		ll_node_t* node = library->buckets[hash_index]->head;

		while (node != NULL) {
			info* data_info = (info *)node->data;

			if (!library->compare_function(data_info->key, book_name)) {
				book_value_t *book = (book_value_t *) data_info->value;

				ht_put(book->ht_def, new_def_key,
					MAX_STRING_SIZE, new_def_val, MAX_STRING_SIZE);

				// checking the load factor once again
				if ((float)book->ht_def->size / (float)book->ht_def->hmax > 1.0) {
					resize_for_def(book->ht_def);
				}

				return;
			}
			node = node->next;
		}
	}
}

// function that removes a definition from a book
void
rmv_def(hashtable_t *library, char book_name[], char def_key[])
{
	if (!ht_has_key(library, book_name)) {
		printf("%s", NOT_HERE);
		return;

	} else {
		book_value_t *book = (book_value_t *)ht_get(library, book_name);

		// checking if the book has the given definition
		if (!ht_has_key(book->ht_def, def_key)) {
			printf("The definition is not in the book.\n");
			return;
		}

		// deleting the pair key-value using the generic hashtable functions
		ht_remove_entry(book->ht_def, def_key);
	}
}

// function that eliberates all of the memory used for a library
void
free_library(hashtable_t *library)
{
	if (!library) {
		return;
	}

	// going through every book to eliberate the memory used for it
	for (unsigned int i = 0; i < library->hmax; ++i) {
		ll_node_t* node = library->buckets[i]->head;

		while (node != NULL) {
			info* data_info = (info *)node->data;
			free(data_info->key);

			book_value_t *val = (book_value_t *)data_info->value;
			ht_free(val->ht_def);
			free(val);

			node = node->next;
		}

		ll_free(&library->buckets[i]);
	}

	free(library->buckets);
	free(library);
}

// function that sorts the books considering the given indications
void
sort_books(linked_list_t *books)
{
	ll_node_t *curr1 = books->head;
	ll_node_t *curr2;

	info *temp;

	while (curr1 != NULL) {
		curr2 = curr1->next;

		while (curr2 != NULL) {
			// getting the informations from two consecutive buckets
			info *inf1 = (info *)curr1->data;
			info *inf2 = (info *)curr2->data;

			// getting the keys and values from the data
			char name1[MAX_STRING_SIZE], name2[MAX_STRING_SIZE];
			snprintf(name1, MAX_STRING_SIZE, "%s", (char *)inf1->key);
			snprintf(name2, MAX_STRING_SIZE, "%s", (char *)inf2->key);

			book_value_t *val1 = (book_value_t *)inf1->value;
			book_value_t *val2 = (book_value_t *)inf2->value;

			// calculating eachs rating
			float rating1, rating2;

			if (val1->loans != 0) {
				rating1 = val1->score / val1->loans;
			} else {
				rating1 = 0;
			}

			if (val2->loans != 0) {
				rating2 = val2->score / val2->loans;
			} else {
				rating2 = 0;
			}

			// swapping the books according to rating, loans and finally name
			if (rating2 > rating1) {
				temp = curr1->data;
				curr1->data = curr2->data;
				curr2->data = temp;

			} else if (rating2 == rating1) {
				if (val2->loans > val1->loans) {
					temp = curr1->data;
					curr1->data = curr2->data;
					curr2->data = temp;

				} else if (val2->loans == val1->loans) {
					if (strcmp(name2, name1) < 0) {
						temp = curr1->data;
						curr1->data = curr2->data;
						curr2->data = temp;
					}
				}
			}
			curr2 = curr2->next;
		}
		curr1 = curr1->next;
	}
}
