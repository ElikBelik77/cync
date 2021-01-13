#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "structs/dictionary.h"

unsigned int hash(char* key, size_t max_hash) {
	unsigned int hash_value = 0;
	for (; *key != '\0'; key++) {
		hash_value += *key + 31*hash_value;
	}
	return hash_value % max_hash;
}

dictionary_t* dictionary_init(size_t dict_size) {
	dictionary_t* dict = (dictionary_t*)malloc(sizeof(dictionary_t));
	dict->size = dict_size;
	dict->nodes = (dict_node_t**)malloc(dict_size*sizeof(dict_node_t*));
	for (int i = 0; i < dict_size; i++) {
		dict->nodes[i] = (dict_node_t*)malloc(sizeof(dict_node_t));
		memset(dict->nodes[i], 0, sizeof(dict_node_t));
		dict->nodes[i]->key = (char*)malloc(8);
		memset(dict->nodes[i]->key, 0, 8);
	}
	return dict;
}

void dictionary_insert(dictionary_t* dict, char* key, void* value, void (*free_ptr)(void* value)) {
	unsigned int hash_value = hash(key, dict->size);
	dict_node_t* entry = dict->nodes[hash_value];
	dict_node_t* ptr = entry;
	for(; ptr != NULL && strcmp(ptr->key, key); ptr = ptr->next);
	if (ptr == NULL) {
		ptr = (dict_node_t*)malloc(sizeof(dict_node_t));
		ptr->next = entry;
		dict->nodes[hash_value] = ptr;
		ptr->key = (char*)malloc(strlen(key)+1);
		ptr->free_ptr = free_ptr;
		fflush(stdout);
		strcpy(ptr->key, key);
	}
	ptr->value = value;
}

void dictionary_print(dictionary_t* dict) {
	dict_node_t* ptr;
	for (int i = 0; i < dict->size; i++) {
		ptr = dict->nodes[i];
		while (ptr->key != NULL && ptr->next != NULL) {
			printf("<%s, %p>\n", ptr->key, ptr->value);
			ptr = ptr->next;
		}
	}
}

void* dictionary_get(dictionary_t* dict, char* key) {
	unsigned int hash_value = hash(key, dict->size);
	dict_node_t* ptr = dict->nodes[hash_value];
	for(; ptr != NULL && strcmp(ptr->key, key); ptr = ptr->next);
	if (ptr == NULL)
		return NULL;
	return ptr->value;
}

void dictionary_free(dictionary_t* dict) {
	for(int i = 0; i < dict->size; i++) {
		dict_node_t* entry = dict->nodes[i];
		dict_node_t* ptr = entry;
		while(entry != NULL) {
			ptr = entry->next;
			if (entry->free_ptr != NULL && entry->value != NULL) {
				entry->free_ptr(entry->value);
			}
			if (entry->key != NULL) {
				free(entry->key);
			}
			free(entry);
			entry = ptr;
		}
	}
	free(dict->nodes);
	free(dict);
}
