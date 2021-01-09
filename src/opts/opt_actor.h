#ifndef OPTACTOR_H_
#define OPTACTOR_H_
#include "../structs/dictionary.h"
#include "../net/net.h"
typedef struct Options_t {
	char* source_file;
	char* dest_host;
	char* type;
} Options;
Options* parse_options(Dictionary* conf, int argc, char** argv);
void act(NetWorker* net_worker, Options* opt);
#endif
