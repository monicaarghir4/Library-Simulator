// Copyright 2021-2022 Arghir Monica-Andreea

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

int compare_function_strings(void *a, void *b);
unsigned int hash_function_string(void *a);
hashtable_t * ht_create(unsigned int hmax,
    unsigned int (*hash_function)(void*),
    int (*compare_function)(void*, void*));
int ht_has_key(hashtable_t *ht, void *key);
void * ht_get(hashtable_t *ht, void *key);
void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size);
void ht_remove_entry(hashtable_t *ht, void *key);
void ht_free(hashtable_t *ht);
void resize_for_def(hashtable_t *defs);
void resize_for_lib(hashtable_t *library);
void resize_for_users(hashtable_t *users);

#endif  // HASHTABLE_H_
