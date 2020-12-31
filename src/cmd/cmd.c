#include "cmd.h"
#include "../structs/dictionary.h"

typedef struct cmd_t {
	void(*cmd_ptr)(Dictionary* args);
	char** arg_names;
	int argc;
} Command;
void command_free(Command* cmd) {
	for (int i = 0; i < cmd->argc; i++) {
		free(cmd->arg_names[i]);
	}
	free(cmd->arg_names);
	free(cmd);
}
void register_cmd(Dictionary* cmd_dict, void (*cmd)(Dictionary* args), char** arg_names) {
	Command* c_struct = (Command*)malloc(sizeof(Command));
	c_struct->cmd_ptr = cmd;
	c_struct->arg_names = arg_names;
//	c_struct->argc = argc;
//	dictionary_insert(cmd_dict, c_struct);
}

void execute(Dictionary* cmd_dict, Dictionary* conf, char** argv) {
	Command* cmd = (Command*)dictionary_get(cmd_dict, argv[0]);
	
}
