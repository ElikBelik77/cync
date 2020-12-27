#ifndef DICTIONARY_H_
#define DICTIONARY_H_
#include <stdbool.h>
typedef struct DictNode_t {
	char* key;
	void* value;
	struct DictNode_t* next;
	void (*free_ptr)(void* value);
} DictNode;


typedef struct Dictionary_t {
	size_t size;
	DictNode** nodes;
} Dictionary;

Dictionary* dictionary_init(size_t dict_size);
void dictionary_insert(Dictionary* dict, char* key, void* value, void (*free_ptr)(void* value));
void* dictionary_get(Dictionary* dict, char* key);
void dictionary_free(Dictionary* dict);
#endif
