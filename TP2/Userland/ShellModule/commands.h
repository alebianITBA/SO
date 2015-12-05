
#ifndef COMMANDS_H
#define COMMANDS_H

#include "string.h"

typedef struct {
	char *name;
	char *help;
	task_entry_point func;
} command_t;

typedef struct {
	command_t **commands;
	uint64_t count;
} command_list_t;

#define COMMANDS_LIST_SIZE 64

#define DECLARE_COMMAND(command) void command_ ##command ##_init();\
uint64_t command_ ##command(int argc, char** argv);

#define COMMAND_HELP(command, _help) static const char* COMMAND_##command##_HELP = _help

#define COMMAND_START(command) \
void command_ ##command ##_init() {\
	extern command_list_t *cmdlist;\
	cmdlist->commands[cmdlist->count] = (command_t*) malloc(sizeof(command_t)); \
	cmdlist->commands[cmdlist->count]->func =  (task_entry_point) command_ ##command;\
	cmdlist->commands[cmdlist->count]->name = (char*) malloc(strlen(#command)+1);\
	cmdlist->commands[cmdlist->count]->help = (char*) malloc(strlen(COMMAND_##command##_HELP)+1);\
	strcpy(cmdlist->commands[cmdlist->count]->name, #command);\
	strcpy(cmdlist->commands[cmdlist->count]->help, COMMAND_##command##_HELP);\
	cmdlist->count++;\
}\
uint64_t command_ ##command(int argc, char** argv)

#define COMMAND_END() }

#define COMMAND_INIT(command) command_ ##command ##_init()

#define COMMAND_RUN(command, argc, argv) command_ ##command(argc, argv)

DECLARE_COMMAND(echo);
DECLARE_COMMAND(set_distribution);
DECLARE_COMMAND(time);
DECLARE_COMMAND(user_name);
DECLARE_COMMAND(host_name);
DECLARE_COMMAND(color);
DECLARE_COMMAND(refresh);
DECLARE_COMMAND(screensaver);
DECLARE_COMMAND(exit);
DECLARE_COMMAND(clear);
DECLARE_COMMAND(rawkbd);
DECLARE_COMMAND(help);
DECLARE_COMMAND(ps);
DECLARE_COMMAND(sigsend);
DECLARE_COMMAND(sigrcv);
DECLARE_COMMAND(cat);
DECLARE_COMMAND(ls);
DECLARE_COMMAND(filesend);
DECLARE_COMMAND(filercv);
DECLARE_COMMAND(mkdir);
DECLARE_COMMAND(edit);
DECLARE_COMMAND(mtest);
DECLARE_COMMAND(shmget);
DECLARE_COMMAND(shmread);
DECLARE_COMMAND(shmwrite);
DECLARE_COMMAND(shmctl);
DECLARE_COMMAND(shmstatus);
DECLARE_COMMAND(shmfree);
DECLARE_COMMAND(semget);
DECLARE_COMMAND(semwait);
DECLARE_COMMAND(semsig);
DECLARE_COMMAND(semstatus);
DECLARE_COMMAND(ipcs);
DECLARE_COMMAND(producer);
DECLARE_COMMAND(consumer);

#endif
