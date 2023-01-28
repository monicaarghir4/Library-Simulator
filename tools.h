// Copyright 2021-2022 Arghir Monica-Andreea

#ifndef TOOLS_H_
#define TOOLS_H_

#define HMAX 100
#define MAX_STRING_SIZE 64
#define MAX_LINE_SIZE 128
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define NOT_HERE "The book is not in the library.\n"
#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ", __FILE__,		\
				__LINE__);				\
			perror(call_description);			\
			exit(errno);					\
		}							\
	} while (0)

// data struture for hashtable key and value
typedef struct info info;
struct info
{
	void *key;
	void *value;
};

// data structures for linked-lists
typedef struct ll_node_t ll_node_t;
struct ll_node_t
{
	void* data;
	ll_node_t* next;
};

typedef struct linked_list_t linked_list_t;
struct linked_list_t
{
	ll_node_t* head;
	unsigned int data_size;
	unsigned int size;
};

// data structure for hashtables
typedef struct hashtable_t hashtable_t;
struct hashtable_t
{
	linked_list_t **buckets;
	unsigned int size;
	unsigned int hmax;
	unsigned int (*hash_function)(void*);
	int (*compare_function)(void*, void*);
};

// data structure that contains data for a book
typedef struct book_value_t book_value_t;
struct book_value_t
{
	hashtable_t *ht_def;
	unsigned int loans;
	float score;
	unsigned int reachable;
};

// data structure that contains data for a user
typedef struct user_value_t user_value_t;
struct user_value_t
{
	int user_score;
	unsigned int has_book;
	char users_book[MAX_STRING_SIZE];
	unsigned int banned;
	unsigned int available_days;
};

#endif  // TOOLS_H_
