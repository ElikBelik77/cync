#ifndef CMD_H_
#define CMD_H_
#include "../structs/dictionary.h"

void register_cmd(Dictionary* cmd_dict, void (*cmd)(Dictionary* args), char** arg_names);
void execute(Dictionary* cmd_dict, Dictionary* conf, char** argv);

#endif
