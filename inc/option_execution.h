#pragma once
#ifndef OPTACTOR_H_
#define OPTACTOR_H_
#include <stdbool.h>

#include "net/net.h"
#include "structs/dictionary.h"
/*
        Struct for holding cli program arguments.
*/
typedef struct {
  char *source_file;
  char *dest_host;
  bool transfer;
} options_t;

/*
        This function parsses given cli arguments into Options_t struct.
*/
options_t *parse_options(dictionary_t *conf, int argc, char **argv);

/*
        This function act according to options.
*/
void act(net_worker_t *net_worker, options_t *opt);
#endif
