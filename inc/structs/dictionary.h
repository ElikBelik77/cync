#pragma once
#ifndef DICTIONARY_H_
#define DICTIONARY_H_
#include <stdbool.h>
#include <stdlib.h>

typedef struct DictNode {
	char* key;
	void* value;
	struct DictNode* next;
	void (*free_ptr)(void* value);
	int is_dict:1;
} dict_node_t;


typedef struct {
	size_t size;
	dict_node_t **nodes;
} dictionary_t;

dictionary_t* dictionary_init(size_t dict_size);
void dictionary_insert(dictionary_t *dict, char* key, void* value, void (*free_ptr)(void* value));
void* dictionary_get(dictionary_t *dict, char* key);
void dictionary_free(dictionary_t *dict);
void dictionary_print(dictionary_t *dict);
#endif
