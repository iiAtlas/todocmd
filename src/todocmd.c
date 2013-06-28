#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_DATA 1000

typedef struct {
	int index;
	char *body;
} todo_item;

typedef struct {
	char *type;
	char *args[MAX_DATA];
	int arg_num;
	char *todo_filename;
	char *done_filename;
} todo_command;

void make_command(int argc, char *argv[]);
void command_selector(todo_command *cmd);
void todo_add(todo_command *cmd);
void print_todo(todo_command *cmd, char *prefix);
void todo_help();
void todo_list(todo_command *cmd);
void todo_remove(todo_command *cmd);

void todo_add(todo_command *cmd)
{
	FILE *todo_file;
	char buffer[MAX_DATA];
	int i;

	print_todo(cmd, "ADDING");

	todo_file = fopen(cmd->todo_filename, "a+");
	assert(todo_file);

	fputs("-", todo_file);
	for(i = 2; i < cmd->arg_num; i++) {
		fputs(cmd->args[i], todo_file);
		fputs(" ", todo_file);
	}
	fputs("\n", todo_file);

	fclose(todo_file);
	free(cmd);

	exit(EXIT_SUCCESS);
}

void print_todo(todo_command *cmd, char *prefix)
{
	int i;

	printf("[%s TODO]: \"", prefix);
	for(i = 2; i < cmd->arg_num; i++) {
		if(i < cmd->arg_num - 1) printf("%s ", cmd->args[i]);
		else printf("%s", cmd->args[i]);
	}
	printf("\"\n");
}

void todo_list(todo_command *cmd)
{
	FILE *todo_file;
	char buffer[1000];

	printf("[TODO FILE]: %s\n", cmd->todo_filename);
	todo_file = fopen(cmd->todo_filename, "r");
	assert(todo_file);

	while(fgets(buffer, 1000, todo_file) != NULL) printf("%s", buffer);
	fclose(todo_file);

	printf("[END TODOS]: %s\n", cmd->todo_filename);

	exit(EXIT_SUCCESS);
}

void todo_remove(todo_command *cmd)
{
	FILE *todo_file;
	FILE *tmp;
	char buffer[1000];
	int line = atoi(cmd->args[2]);
	int count = 0;
	int i = 0;

	printf("[DELETING TODO]: %d\n", line);
	todo_file = fopen(cmd->todo_filename, "r");
	tmp = fopen("tmp.txt", "w");

	/* TODO: create external function to allow for use in multiple cases */
	/* Hacked Together Line Counter */
	while((i = fgets(buffer, sizeof(buffer), todo_file) != NULL)) {
		if(i == 1) count++;
		if(count != line) fprintf(tmp, "%s", buffer);
	}

	/* TODO: change to freopen() */
	fclose(todo_file);
	fclose(tmp);
	todo_file = fopen(cmd->todo_filename, "w");
	tmp = fopen("tmp.txt", "r");

	while(fgets(buffer, 1000, tmp) != NULL) {
		fprintf(todo_file, "%s", buffer);
	}

	exit(EXIT_SUCCESS);
}

void todo_help()
{
	printf("[TODOCMD HELP]:\n");

	printf("-Adding Todo: todocmd -a [todo_text]\n");
	printf("-Removing Todo: todocmd -r [todo_index]\n");
	printf("-Listing Todos: todocmd -l\n");
	printf("-Todocmd Help: todocmd -h\n");

	printf("[END TODOCMD HELP]\n");

	exit(EXIT_SUCCESS);
}

void make_command(int argc, char *argv[])
{	
	int i;

	todo_command *cmd = malloc(sizeof(todo_command));
	assert(cmd);

	cmd->type = argv[1];
	for(i = 2; i < argc; i++) {
		cmd->args[i] = argv[i];
	}
	cmd->arg_num = argc;

	cmd->todo_filename = "todo.txt";
	cmd->done_filename = "done.txt";

	command_selector(cmd);
}

void command_selector(todo_command *cmd)
{
	if(strcmp(cmd->type, "-a") == 0) {
		todo_add(cmd);
	} else if(strcmp(cmd->type, "-l") == 0) {
		todo_list(cmd);
	} else if(strcmp(cmd->type, "-r") == 0) {
		todo_remove(cmd);
	} else if(strcmp(cmd->type, "-h") == 0) {
		todo_help();
	} else {
		printf("-todocmd: command not found\nType \"help\" or \"-h\" for help\n");
	}
}

int main(int argc, char *argv[])
{
	if(argc > 1) {
		make_command(argc, argv);
	} else {
		printf("No Input Recieved. Shutting Down.\n");
	}

        exit(EXIT_SUCCESS);
}
