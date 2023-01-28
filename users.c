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

// function that adds a new user to the hashtable
void
add_user(hashtable_t *users, char user_name[])
{
	// checking if the user already exists
	if (ht_has_key(users, user_name)) {
		printf("User is already registered.\n");
		return;

	} else {
		user_value_t *val = malloc(sizeof(user_value_t));
		DIE(!val, "Error");

		// setting the initial chaaracteristics for the user
		val->user_score = 100;
		val->has_book = 0;
		val->banned = 0;
		val->available_days = -1;
		memset(val->users_book, 0, MAX_STRING_SIZE);

		ht_put(users, user_name, MAX_STRING_SIZE,
			val, sizeof(user_value_t));

		free(val);

		// checking if resizing is needed
		if ((float)users->size / (float)users->hmax > 1.0) {
			resize_for_users(users);
		}
	}
}

// function for when a book is borrowed
void
borrow(hashtable_t *library, hashtable_t *users,
	char user_name[], char book_name[], unsigned int days)
{
	user_value_t *user_val = (user_value_t *)ht_get(users, user_name);
	book_value_t *book_val = (book_value_t *)ht_get(library, book_name);

	// checking the cases in which the borrowing can't happen
	if (!ht_has_key(users, user_name)) {
		printf("You are not registered yet.\n");
		return;
	}

	if (user_val->banned) {
		printf("You are banned from this library.\n");
		return;
	}

	if (user_val->has_book) {
		printf("You have already borrowed a book.\n");
		return;
	}

	if (!ht_has_key(library, book_name)) {
		printf("%s", NOT_HERE);
		return;
	}

	if (!book_val->reachable) {
		printf("The book is borrowed.\n");
		return;
	}

	// switching the values of the user
	book_val->reachable = 0;

	user_val->has_book = 1;
	snprintf(user_val->users_book, MAX_STRING_SIZE, "%s", book_name);
	user_val->available_days = days;
}

// function for when a book is returned
void
return_book(hashtable_t *library, hashtable_t *users, char user_name[],
	char book_name[], unsigned int days_since_borrow, int rating)
{
	user_value_t *user_val = (user_value_t *)ht_get(users, user_name);
	book_value_t *book_val = (book_value_t *)ht_get(library, book_name);

	// checking the invalid cases
	if (user_val->banned) {
		printf("You are banned from this library.\n");
		return;
	}

	if (!user_val->has_book || strcmp(user_val->users_book, book_name)) {
		printf("You didn't borrow this book.\n");
		return;
	}

	// calculating the points depending on the days of the loan
	if (user_val->available_days < days_since_borrow) {
		user_val->user_score = user_val->user_score
			- 2 * (days_since_borrow - user_val->available_days);

	} else if (user_val->available_days > days_since_borrow) {
		user_val->user_score = user_val->user_score
			+ (user_val->available_days - days_since_borrow);
	}

	// changing the value of the user and of the book
	user_val->has_book = 0;
	memset(user_val->users_book, 0, MAX_STRING_SIZE);
	user_val->available_days = -1;

	book_val->reachable = 1;
	book_val->loans += 1;
	book_val->score += rating;

	// if the user score is below zero the user is banned
	if (user_val->user_score < 0) {
		user_val->banned = 1;
		printf("The user %s has been banned from this library.\n", user_name);
		return;
	}
}

// function for when a book is lost
void
lost_book(hashtable_t *library, hashtable_t *users,
	char user_name[], char book_name[])
{
	user_value_t *user_val = (user_value_t *)ht_get(users, user_name);

	if (!ht_has_key(users, user_name)) {
		printf("You are not registered yet.\n");
		return;
	}

	if (user_val->banned) {
		printf("You are banned from this library.\n");
		return;
	}

	// changing the user values and deleting the book from the library
	user_val->user_score = user_val->user_score - 50;
	user_val->has_book = 0;
	memset(user_val->users_book, 0, MAX_STRING_SIZE);
	user_val->available_days = -1;


	rmv_book(library, book_name);

	// banning the user if needed
	if (user_val->user_score < 0) {
		user_val->banned = 1;
		printf("The user %s has been banned from this library.\n", user_name);
		return;
	}
}

// function that sorts the users
void
sort_users(linked_list_t *u)
{
	ll_node_t *curr1 = u->head;
	ll_node_t *curr2;

	info *temp;

	while (curr1 != NULL) {
		curr2 = curr1->next;

		while (curr2 != NULL) {
			// getting the data
			info *inf1 = (info *)curr1->data;
			info *inf2 = (info *)curr2->data;

			char name1[MAX_STRING_SIZE], name2[MAX_STRING_SIZE];
			snprintf(name1, MAX_STRING_SIZE, "%s", (char *)inf1->key);
			snprintf(name2, MAX_STRING_SIZE, "%s", (char *)inf2->key);

			user_value_t *val1 = (user_value_t *)inf1->value;
			user_value_t *val2 = (user_value_t *)inf2->value;

			// swapping the users depending on user score and name
			if (val2->user_score > val1->user_score) {
				temp = curr1->data;
				curr1->data = curr2->data;
				curr2->data = temp;

			} else if (val2->user_score == val1->user_score) {
				if (strcmp(name2, name1) < 0) {
					temp = curr1->data;
					curr1->data = curr2->data;
					curr2->data = temp;
				}
			}
			curr2 = curr2->next;
		}
		curr1 = curr1->next;
	}
}
