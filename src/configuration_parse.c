#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configuration_parse.h"

dictionary_t *parse_conf_dict(FILE *f);

// Predicate function for legal characters to appear on key:value entry in the
// configuration.
int isascii_modified(char c) {
	return !isspace(c) && (isalpha(c) || isdigit(c) || c == '{' || c == '.' || c == '_');
}
// Function that skips closing parenthesis and whitespaces.
void file_skip_parenthesis(FILE *f) {
	char current;
	do {
		current = fgetc(f);
	} while (isspace(current) || current == '}');
	fseek(f, -1L, SEEK_CUR);
}

// Function that trims the file from whitespaces.
void file_trim(FILE *f) {
	char current;
	do {
		current = fgetc(f);
	} while (isspace(current));
	fseek(f, -1L, SEEK_CUR);
}

// Calculates the current line length.
int file_line_length(FILE *f) {
	int current_file_offset = ftell(f);
	int count = 0;
	char current;
	do {
		count += 1;
		current = fgetc(f);
	} while (current != '\n');
	fseek(f, current_file_offset, SEEK_SET);
	return count;
}
// Function that read for the given file into the buffer until the predicate is
// false.
void file_read_until(FILE *f, char *buffer, int (*predicate)(char)) {
	int i = 0;
	char current;
	do {
		current = fgetc(f);
		buffer[i++] = current;
	} while (predicate(current));
	buffer[i-1] = '\0';
}

// This function, given a dictionary pointed by f SEEK_CUR, finds the offset of
// its' closing parenthesis.
int find_dict_end(FILE *f) {
	int current_file_offset = ftell(f);
	int offset = 0;
	int pcount = 0;
	do {
		char current = fgetc(f);
		if (current == '{') {
			pcount += 1;
		} else if (current == '}') {
			pcount -= 1;
		}
		offset++;
	} while (pcount);
	fseek(f, current_file_offset + 1, SEEK_SET);
	return offset + current_file_offset - 1;
}
// This function parses a node from the file.
//	node - the node buffer to fill.
//	is_dict - if the parsed node is dictionary *is_dict = 1, else 0.
void parse_node(FILE *f, dict_node_t *node, bool *is_dict) {
	file_trim(f);
	int line_length = file_line_length(f);
	char *node_key = (char *)malloc(line_length);
	char *node_value = (char *)malloc(line_length);
	file_trim(f);
	file_read_until(f, node_key, isascii_modified);
	fseek(f, 1L, SEEK_CUR);
	file_trim(f);
	file_read_until(f, node_value, isascii_modified);
	if (node_value[0] == '{') {
		fseek(f, -strlen(node_value) - 1, SEEK_CUR);
		dictionary_t *nested_dictionary = parse_conf_dict(f);
		node->key = node_key;
		node->value = nested_dictionary;
		free(node_value);
		*is_dict = 1;
		file_trim(f);
		return;
	}
	*is_dict = 0;
	file_trim(f);
	node->key = node_key;
	node->value = node_value;
	return;
}

// This function parses a configuration file into a dictionary.
dictionary_t *parse_conf_dict(FILE *f) {
	dictionary_t *result = dictionary_init(100);
	int dict_end_offset = find_dict_end(f);
	int current_offset;
	while ((current_offset = ftell(f)) < dict_end_offset) {
		// parse nodes, until the current offset is outside of the dictionary.
		dict_node_t *node = (dict_node_t *)malloc(sizeof(dict_node_t));
		bool is_dict;
		parse_node(f, node, &is_dict);
		if (is_dict) {
			dictionary_insert(result, node->key, node->value,
			(void (*)(void *))dictionary_free);
		} else {
			dictionary_insert(result, node->key, node->value, free);
		}
		free(node->key);
		free(node);
		file_skip_parenthesis(f);
	}
	return result;
}

dictionary_t *parse_conf(char *path) {
	FILE *f = fopen(path, "rb");
	if (!f) {
		fprintf(stderr, "file open error\n");
		exit(0);
	}
	dictionary_t *conf_dict = parse_conf_dict(f);
	fclose(f);
	return conf_dict;
}
