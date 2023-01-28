// Copyright 2021-2022 Arghir Monica-Andreea

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "tools.h"
#include "linkedlist.h"
#include "hashtable.h"
#include "input.h"
#include "books.h"
#include "users.h"

// function that prints the informations of a book from the library
void
get_book(hashtable_t *library, char book_name[])
{
	if (!ht_has_key(library, book_name)) {
		printf("%s", NOT_HERE);
		return;

	} else if (ht_has_key(library, book_name)) {
		book_value_t *val = (book_value_t *) ht_get(library, book_name);

		if (val->loans != 0) {
			printf("Name:%s Rating:%0.3f Purchases:%d\n",
				book_name, val->score / val->loans, val->loans);
		} else {
			printf("Name:%s Rating:%0.3f Purchases:%d\n",
				book_name, val->score, val->loans);
		}
	}
}

// function that prints the definitions value from a book
void
get_def(hashtable_t *library, char book_name[], char def_key[])
{
	if (!ht_has_key(library, book_name)) {
		printf("%s", NOT_HERE);
		return;

	} else {
		book_value_t *book = (book_value_t *)ht_get(library, book_name);

		if (!ht_has_key(book->ht_def, def_key)) {
			printf("The definition is not in the book.\n");
			return;
		}

		char *def_value = (char *)ht_get(book->ht_def, def_key);
		printf("%s\n", def_value);
	}
}

// function that prints the clasament of the books
void
top_books(hashtable_t *library)
{
	// creating a new list with all the books to sort it
	linked_list_t *books = ll_create(sizeof(info));

	for (unsigned int i = 0; i < library->hmax; i++) {
		linked_list_t *book = library->buckets[i];

		ll_node_t *curr = book->head;

		while (curr) {
			info *data = (info *)curr->data;
			ll_add_nth_node(books, 0, data);

			curr = curr->next;
		}
	}

	sort_books(books);

	printf("Books ranking:\n");

	ll_node_t *curr = books->head;
	int i = 1;

	while (curr) {
		info *inf = (info *)curr->data;
		char name[MAX_STRING_SIZE];
		snprintf(name, MAX_STRING_SIZE, "%s", (char *)inf->key);
		book_value_t *val = (book_value_t *)inf->value;

		if (val->loans != 0) {
			printf("%d. Name:%s Rating:%0.3f Purchases:%d\n",
				i, name, val->score / val->loans, val->loans);

		} else {
			printf("%d. Name:%s Rating:%0.3f Purchases:%d\n",
				i, name, val->score, val->loans);
		}

		i++;
		curr = curr->next;
	}

	ll_free(&books);
}

// function that prints the clasament of the users that follows
// the same algorithm
void
top_users(hashtable_t *users)
{
	linked_list_t *u = ll_create(sizeof(info));

	for (unsigned int i = 0; i < users->hmax; i++) {
		linked_list_t *user = users->buckets[i];

		ll_node_t *curr = user->head;

		while (curr) {
			info *data = (info *)curr->data;
			ll_add_nth_node(u, 0, data);

			curr = curr->next;
		}
	}

	sort_users(u);

	printf("Users ranking:\n");

	ll_node_t *curr = u->head;
	int i = 1;

	while (curr) {
		info *inf = (info *)curr->data;
		char name[MAX_STRING_SIZE];
		snprintf(name, MAX_STRING_SIZE, "%s", (char *)inf->key);
		user_value_t *val = (user_value_t *)inf->value;

		if (val->banned) {
			i++;
			curr = curr->next;
			continue;
		}

		printf("%d. Name:%s Points:%d\n",
			i, name, val->user_score);

		i++;
		curr = curr->next;
	}

	ll_free(&u);
}
