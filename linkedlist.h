// Copyright 2021-2022 Arghir Monica-Andreea

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

linked_list_t* ll_create(unsigned int data_size);
ll_node_t* get_nth_node(linked_list_t* list, unsigned int n);
ll_node_t* create_node(const void* new_data, unsigned int data_size);
void ll_add_nth_node(linked_list_t* list, unsigned int n,
    const void* new_data);
ll_node_t* ll_remove_nth_node(linked_list_t* list, unsigned int n);
void ll_free(linked_list_t** pp_list);

#endif  // LINKEDLIST_H_
