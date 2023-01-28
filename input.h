// Copyright 2021-2022 Arghir Monica-Andreea

#ifndef INPUT_H_
#define INPUT_H_

void get_lost_args(char command[], char user_name[], char book_name[]);
void get_return_args(char command[], char user_name[], char book_name[],
    unsigned int *days_since_borrow, int *rating);
void get_borrow_args(char command[], char user_name[], char book_name[],
	unsigned int *days);
void get_two_strings_arg(char command[], char book_name[], char def_key[]);
void get_new_def(char command[], char book_name[], char new_def_key[],
    char new_def_val[]);
void get_string(char command[], char book_name[]);
void get_arg_add_book(char command[], char book_name[], int *def_number);

#endif  // INPUT_H_
