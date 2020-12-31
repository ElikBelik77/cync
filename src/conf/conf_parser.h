#ifndef CONF_PARSER_H_
#define CONF_PARSER_H_
#include "../structs/dictionary.h"

/*
	This function parses a configuration file into a dictionary.
		path - the path to the configuration file.
*/
Dictionary* parse_conf(char* path);

#endif
