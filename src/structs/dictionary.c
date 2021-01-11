#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "structs/dictionary.h"

unsigned int hash(char* key, size_t max_hash) {
	fflush(stdout);
	unsigned int hash_value = 0;
	for (; *key != '\0'; key++) {
		hash_value += *key + 31*hash_value;
	}
	return hash_value % max_hash;
}

Dictionary* dictionary_init(size_t dict_size) {
	Dictionary* dict = (Dictionary*)malloc(sizeof(Dictionary));
	dict->size = dict_size;
	dict->nodes = (DictNode**)malloc(dict_size*sizeof(DictNode*));
	for (int i = 0; i < dict_size; i++) {
		dict->nodes[i] = (DictNode*)malloc(sizeof(DictNode));
		memset(dict->nodes[i], 0, sizeof(DictNode));
		dict->nodes[i]->key = (char*)malloc(8);
		memset(dict->nodes[i]->key, 0, 8);
	}
	return dict;
}

void dictionary_insert(Dictionary* dict, char* key, void* value, void (*free_ptr)(void* value)) {
	unsigned int hash_value = hash(key, dict->size);
	DictNode* entry = dict->nodes[hash_value];
	DictNode* ptr = entry;
	for(; ptr != NULL && strcmp(ptr->key, key); ptr = ptr->next);
	if (ptr == NULL) {
		ptr = (DictNode*)malloc(sizeof(DictNode));
		ptr->next = entry;
		dict->nodes[hash_value] = ptr;
		ptr->key = (char*)malloc(strlen(key)+1);
		ptr->free_ptr = free_ptr;
		fflush(stdout);
		strcpy(ptr->key, key);
	}
	ptr->value = value;
}

void dictionary_print(Dictionary* dict) {
	DictNode* ptr;
	for (int i = 0; i < dict->size; i++) {
		ptr = dict->nodes[i];
		while (ptr->key != NULL && ptr->next != NULL) {
			printf("<%s, %p>\n", ptr->key, ptr->value);
			ptr = ptr->next;
		}
	}
	fflush(stdout);
}

void* dictionary_get(Dictionary* dict, char* key) {
	unsigned int hash_value = hash(key, dict->size);
	DictNode* ptr = dict->nodes[hash_value];
	for(; ptr != NULL && strcmp(ptr->key, key); ptr = ptr->next);
	if (ptr == NULL)
		return NULL;
	return ptr->value;
}

void dictionary_free(Dictionary* dict) {
	for(int i = 0; i < dict->size; i++) {
		DictNode* entry = dict->nodes[i];
		DictNode* ptr = entry;
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
