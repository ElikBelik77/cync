#ifndef OPTACTOR_H_
#define OPTACTOR_H_
#include "../structs/dictionary.h"
#include "../net/net.h"
/*
	Struct for holding cli program arguments.
*/
typedef struct Options_t {
	char* source_file;
	char* dest_host;
	char* type;
} Options;

/*
	This function parsses given cli arguments into Options_t struct.
*/
Options* parse_options(Dictionary* conf, int argc, char** argv);

/*
	This function act according to options.
*/
void act(NetWorker* net_worker, Options* opt);
#endif
