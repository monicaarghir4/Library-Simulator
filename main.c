// Copyright 2021-2022 Arghir Monica-Andreea

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "tools.h"
#include "input.h"
#include "linkedlist.h"
#include "hashtable.h"
#include "users.h"
#include "books.h"
#include "output.h"

int main(void)
{
	char command[MAX_LINE_SIZE];

	// creating 2 hashtables for the library and for the users
	hashtable_t *library =
		ht_create(10, hash_function_string, compare_function_strings);

	hashtable_t *users =
		ht_create(10, hash_function_string, compare_function_strings);

	while (1) {
		// getting the command from the input
		memset(command, 0, MAX_LINE_SIZE);
		fgets(command, MAX_LINE_SIZE, stdin);
		if (strcmp(command, "EXIT")) {
			command[strlen(command) - 1] = '\0';
		}

		// treating each case of the commands and for each one
		// getting the arguments
		if (!strncmp(command, "ADD_BOOK", 8)) {
			char book_name[MAX_STRING_SIZE];
			int def_number;

			memmove(command, command + 9, strlen(command));
			get_arg_add_book(command, book_name, &def_number);

			add_book(library, book_name, def_number);

		} else if (!strncmp(command, "GET_BOOK", 8)) {
			char book_name[MAX_STRING_SIZE];

			memmove(command, command + 9, strlen(command));
			get_string(command, book_name);

			get_book(library, book_name);

		} else if (!strncmp(command, "RMV_BOOK", 8)) {
			char book_name[MAX_STRING_SIZE];

			memmove(command, command + 9, strlen(command));
			get_string(command, book_name);

			rmv_book(library, book_name);

		} else if (!strncmp(command, "ADD_DEF", 7)) {
			char book_name[MAX_STRING_SIZE];
			char new_def_key[MAX_STRING_SIZE];
			char new_def_val[MAX_STRING_SIZE];

			memmove(command, command + 8, strlen(command));
			get_new_def(command, book_name, new_def_key, new_def_val);

			add_def(library, book_name, new_def_key, new_def_val);

		} else if (!strncmp(command, "GET_DEF", 7)) {
			char book_name[MAX_STRING_SIZE];
			char def_key[MAX_STRING_SIZE];

			memmove(command, command + 8, strlen(command));
			get_two_strings_arg(command, book_name, def_key);

			get_def(library, book_name, def_key);

		} else if (!strncmp(command, "RMV_DEF", 7)) {
			char book_name[MAX_STRING_SIZE];
			char def_key[MAX_STRING_SIZE];

			memmove(command, command + 8, strlen(command));
			get_two_strings_arg(command, book_name, def_key);

			rmv_def(library, book_name, def_key);

		} else if (!strncmp(command, "ADD_USER", 8)) {
			char user_name[MAX_STRING_SIZE];

			memmove(command, command + 9, strlen(command));
			get_string(command, user_name);

			add_user(users, user_name);

		} else if (!strncmp(command, "BORROW", 6)) {
			char user_name[MAX_STRING_SIZE];
			char book_name[MAX_STRING_SIZE];
			unsigned int days;

			memmove(command, command + 7, strlen(command));
			get_borrow_args(command, user_name, book_name, &days);

			borrow(library, users, user_name, book_name, days);

		} else if (!strncmp(command, "RETURN", 6)) {
			char user_name[MAX_STRING_SIZE];
			char book_name[MAX_STRING_SIZE];
			unsigned int days_since_borrow;
			int rating;

			memmove(command, command + 7, strlen(command));
			get_return_args(command, user_name, book_name,
				&days_since_borrow, &rating);

			return_book(library, users, user_name,
				book_name, days_since_borrow, rating);

		} else if (!strncmp(command, "LOST", 4)) {
			char user_name[MAX_STRING_SIZE];
			char book_name[MAX_STRING_SIZE];

			memmove(command, command + 5, strlen(command));

			get_lost_args(command, user_name, book_name);

			lost_book(library, users, user_name, book_name);

		} else if (!strncmp(command, "EXIT", 4)) {
			top_books(library);
			top_users(users);

			// eliberating the used memory
			free_library(library);
			ht_free(users);

			break;
		}
	}
	return 0;
}
