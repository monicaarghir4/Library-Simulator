// Copyright 2021-2022 Arghir Monica-Andreea

#ifndef USERS_H_
#define USERS_H_

void add_user(hashtable_t *users, char user_name[]);
void borrow(hashtable_t *library, hashtable_t *users,
	char user_name[], char book_name[], unsigned int days);
void return_book(hashtable_t *library, hashtable_t *users, char user_name[],
	char book_name[], unsigned int days_since_borrow, int rating);
void lost_book(hashtable_t *library, hashtable_t *users,
	char user_name[], char book_name[]);
void sort_users(linked_list_t *u);

#endif  // USERS_H_
