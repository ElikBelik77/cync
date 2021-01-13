#pragma once
#ifndef CONFIGURATION_PARSE_H_
#define CONFIGURATION_PARSE_H_
#include "structs/dictionary.h"

/*
        This function parses a configuration file into a dictionary.
                path - the path to the configuration file.
*/
dictionary_t *parse_conf(char *path);

#endif
