#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "conf_parser.h"
#include "../structs/dictionary.h"
Dictionary* parse_conf_dict(FILE** f);

int isascii_modified(char c) {
	return !isspace(c) && isascii(c);
}
void file_trim(FILE** f) {
	char current;
	do {
		current = fgetc(*f);
	} while(isspace(current));
	fseek(*f, -1L, SEEK_CUR);
}
int file_line_length(FILE** f) {
	int current_file_offset = ftell(*f);
	int count = -1;
	char current;
	do {
		count += 1;
		current = fgetc(*f);
	} while(current != '\n');
	fseek(*f, current_file_offset, SEEK_SET);
	return count;
}
void file_read_until(FILE** f, char* buffer, int(*predicate)(char)) {
	int i = 0;
	char current;
	do {
		current = fgetc(*f);
		buffer[i] = current;
	} while(predicate(current));
	buffer[i] = '\0';
}
int find_dict_end(FILE** f) {
	int current_file_offset = ftell(*f);
	int offset;
	int pcount = 0;
	do {
		file_trim(f);
		char current = fgetc(*f);
		pcount += current == '{';
		pcount -= current == '}';
		offset ++;
	} while(pcount);

	fseek(*f, current_file_offset, SEEK_SET);
	return offset+current_file_offset;
}
void parse_node(FILE** f, DictNode* node, bool* is_dict) {
	int line_length = file_line_length(f);
	char* node_key = (char*)malloc(line_length);
	char* node_value = (char*)malloc(line_length);
	file_trim(f);
	file_read_until(f, node_key, isascii_modified);
	file_trim(f);
	file_read_until(f, node_value, isascii_modified);
	if (node_value[0] = '{') {
		fseek(*f, -1L, SEEK_CUR);
		Dictionary* nested_dictionary = parse_conf_dict(f);
		node->key = node_key;
		node->value = nested_dictionary;
		*is_dict = 1;
		return;
	}
	file_trim(f);
	node->key = node_key;
	node->value = node_value;
	return;
}
Dictionary* parse_conf_dict(FILE** f) {
	Dictionary* result = dictionary_init(100);
	int dict_end_offset = find_dict_end(f);
	int current_offset;
	while  ((current_offset = ftell(*f)) != dict_end_offset) {
		DictNode* node = (DictNode*)malloc(sizeof(DictNode));
		bool is_dict;
		parse_node(f, node, &is_dict);
		if (is_dict)
			dictionary_insert(result, node->key, node->value, (void(*)(void*))dictionary_free);
		else
			dictionary_insert(result, node->key, node->value, free);
		free(node->key);
		free(node);
	}

}
#if defined(unix) || defined(__unix__) || defined(__unix)
Dictionary* parse_conf(char* path) {
	FILE *f = fopen(path, "rb");
	return parse_conf_dict(&f);
}

#endif

#if defined(WIN32)
Dictionary* parse_conf(char* path) {
	return NULL;
}

#endif
