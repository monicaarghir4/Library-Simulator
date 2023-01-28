// Copyright 2021-2022 Arghir Monica-Andreea

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "tools.h"

// functions that get the arguments for each command
void
get_arg_add_book(char command[], char book_name[], int *def_number)
{
	// checking if the book name is between " "
	if (command[0] == '\"') {
		// moving the whole string to the right
		memmove(command, command + 1, strlen(command));
		// copying in the book name string the whole command and stopping
		// when it finds the end of the name
		snprintf(book_name, MAX_STRING_SIZE, "%s", command);

		int i = 0;
		while (book_name[i] != '\"') {
			i++;
		}

		book_name[i] = '\0';

		memmove(command, command + i + 1, strlen(command));
		*def_number = atoi(command);

	} else {
		char *token;
		int args = 1;

		while ((token = __strtok_r(command, " ", &command))) {
			if (args == 1) {
				snprintf(book_name, MAX_STRING_SIZE, "%s", token);
				args++;

			} else {
				*def_number = atoi(token);
			}
		}
	}
}

void
get_string(char command[], char book_name[])
{
	 if (command[0] == '\"') {
		memmove(command, command + 1, strlen(command));
		snprintf(book_name, MAX_STRING_SIZE, "%s", command);

		int i = 0;
		while (book_name[i] != '\"') {
			i++;
		}

		book_name[i] = '\0';

	} else {
		char *token;
		int args = 1;

		while ((token = __strtok_r(command, " ", &command))) {
			if (args == 1) {
				snprintf(book_name, MAX_STRING_SIZE, "%s", token);
				args++;
			}
		}
	}
}

void
get_new_def(char command[], char book_name[],
				char new_def_key[], char new_def_val[])
{
	if (command[0] == '\"') {
		memmove(command, command + 1, strlen(command));
		snprintf(book_name, MAX_STRING_SIZE, "%s", command);

		int i = 0;
		while (book_name[i] != '\"') {
			i++;
		}

		book_name[i] = '\0';

		memmove(command, command + i + 2, strlen(command));

		snprintf(new_def_key, MAX_STRING_SIZE, "%s", command);

		i = 0;
		while (new_def_key[i] != ' ') {
			i++;
		}

		new_def_key[i] = '\0';

		memmove(command, command + i + 1, strlen(command));

		snprintf(new_def_val, MAX_STRING_SIZE, "%s", command);

	} else {
		char *token;
		int args = 1;

		while ((token = __strtok_r(command, " ", &command))) {
			if (args == 1) {
				snprintf(book_name, MAX_STRING_SIZE, "%s", token);
				args++;

			} else if (args == 2) {
				snprintf(new_def_key, MAX_STRING_SIZE, "%s", token);
				args++;

			} else {
				snprintf(new_def_val, MAX_STRING_SIZE, "%s", token);
			}
		}
	}
}

void
get_two_strings_arg(char command[], char book_name[], char def_key[])
{
	if (command[0] == '\"') {
		memmove(command, command + 1, strlen(command));

		snprintf(book_name, MAX_STRING_SIZE, "%s", command);

		int i = 0;
		while (book_name[i] != '\"') {
			i++;
		}

		book_name[i] = '\0';

		memmove(command, command + i + 2, strlen(command));

		snprintf(def_key, MAX_STRING_SIZE, "%s", command);

	} else {
		char *token;
		int args = 1;

		while ((token = __strtok_r(command, " ", &command))) {
			if (args == 1) {
				snprintf(book_name, MAX_STRING_SIZE, "%s", token);
				args++;

			} else if (args == 2) {
				snprintf(def_key, MAX_STRING_SIZE, "%s", token);
			}
		}
	}
}

void
get_borrow_args(char command[], char user_name[], char book_name[],
	unsigned int *days)
{
	snprintf(user_name, MAX_STRING_SIZE, "%s", command);

	int i = 0;
	while (user_name[i] != ' ') {
		i++;
	}

	user_name[i] = '\0';

	memmove(command, command + i + 1, strlen(command));

	snprintf(book_name, MAX_STRING_SIZE, "%s", command);

	if (book_name[0] == '\"') {
		memmove(book_name, book_name + 1, strlen(book_name));

		i = 0;
		while (book_name[i] != '\"') {
			i++;
		}

		book_name[i] = '\0';

		memmove(command, command + i + 2, strlen(command));

	} else {
		i = 0;
		while (book_name[i] != ' ') {
			i++;
		}

		book_name[i] = '\0';

		memmove(command, command + i + 1, strlen(command));
	}

	*days = atoi(command);
}

void
get_return_args(char command[], char user_name[], char book_name[],
	unsigned int *days_since_borrow, int *rating)
{
	snprintf(user_name, MAX_STRING_SIZE, "%s", command);

	int i = 0;
	while (user_name[i] != ' ') {
		i++;
	}

	user_name[i] = '\0';

	memmove(command, command + i + 1, strlen(command));

	snprintf(book_name, MAX_STRING_SIZE, "%s", command);

	if (book_name[0] == '\"') {
		memmove(book_name, book_name + 1, strlen(book_name));

		i = 0;
		while (book_name[i] != '\"') {
			i++;
		}

		book_name[i] = '\0';

		memmove(command, command + i + 2, strlen(command));

	} else {
		i = 0;
		while (book_name[i] != ' ') {
			i++;
		}

		book_name[i] = '\0';

		memmove(command, command + i + 1, strlen(command));
	}

	char *token;
	int cnt = 1;

	while ((token = __strtok_r(command, " ", &command))) {
		if (cnt == 1) {
			*days_since_borrow = atoi(token);
			cnt++;

		} else {
			*rating = atoi(token);
		}
	}
}

void
get_lost_args(char command[], char user_name[], char book_name[])
{
	snprintf(user_name, MAX_STRING_SIZE, "%s", command);

	int i = 0;
	while (user_name[i] != ' ') {
		i++;
	}

	user_name[i] = '\0';

	memmove(command, command + i + 1, strlen(command));

	snprintf(book_name, MAX_STRING_SIZE, "%s", command);

	if (book_name[0] == '\"') {
		memmove(book_name, book_name + 1, strlen(book_name));

		i = 0;
		while (book_name[i] != '\"') {
			i++;
		}

		book_name[i] = '\0';

	} else {
		i = 0;
		while (book_name[i] != ' ') {
			i++;
		}

		book_name[i] = '\0';
	}
}
