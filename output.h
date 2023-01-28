// Copyright 2021-2022 Arghir Monica-Andreea

#ifndef OUTPUT_H_
#define OUTPUT_H_

void get_book(hashtable_t *library, char book_name[]);
void get_def(hashtable_t *library, char book_name[], char def_key[]);
void top_books(hashtable_t *library);
void top_users(hashtable_t *users);

#endif  // OUTPUT_H_
