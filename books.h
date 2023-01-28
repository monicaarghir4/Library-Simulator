// Copyright 2021-2022 Arghir Monica-Andreea

#ifndef BOOKS_H_
#define BOOKS_H_

void rmv_book(hashtable_t *library, char book_name[]);
void add_book(hashtable_t *library, char book_name[], int def_number);
void add_def(hashtable_t *library, char book_name[],
	char new_def_key[], char new_def_val[]);
void rmv_def(hashtable_t *library, char book_name[], char def_key[]);
void free_library(hashtable_t *library);
void sort_books(linked_list_t *books);

#endif  // BOOKS_H_
