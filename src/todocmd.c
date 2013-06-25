#include <stdio.h>
#include <stdlib.h>

void todo_add(int argc, char *argv[]);
void todo_list();
void todo_remove(int line);
void todo_help();

typedef struct {
	int index;
	char *body;
} todo_item;

typedef struct {
	char *type;
	char *args;
} todo_command;

/* TODO: change to switch statement */
void parse_input(int argc, char *argv[])
{	
	todo_command cmd;
	cmd.type = argv[1];
	if(strcmp(argv[1], "-a") == 0) {
		todo_add(argc, argv);
	} else if(strcmp(argv[1], "-l") == 0) {
		todo_list();
	} else if(strcmp(argv[1], "-r") == 0 && argv[2] != NULL) {
		int line;
		line = atoi(argv[2]);
		todo_remove(line);
	} else if(strcmp(argv[1], "-h") == 0) {
		todo_help();
	} else {
		printf("-todocmd: command not found\nType \"help\" or \"-h\" for help\n");
	}
}

void todo_add(int argc, char *argv[])
{
	FILE *todo_file;
	char todo_buffer[1000];
	int i;

	printf("[TODOCMD]: Adding todo: \"");
	for(i = 2; i < argc; i++) {
		if(i < argc-1) {
			printf("%s ", argv[i]);
		} else {
			printf("%s", argv[i]);
		}
	}
	printf("\"\n");

	todo_file = fopen("todo.txt", "a+");
	if(!todo_file) exit(EXIT_FAILURE);

	if(argv[2] == NULL) exit(EXIT_FAILURE);
	fputs("-", todo_file);
	for(i = 2; i <= argc; i++) {
		if(i < argc) {
			fputs(argv[i], todo_file);
			fputs(" ", todo_file);
		} else {
			fputs("\n", todo_file);
		}
	}
	fclose(todo_file);

	exit(EXIT_SUCCESS);
}

void todo_list()
{
	FILE *todo_file;
	char buffer[1000];

	printf("[TODOCMD]: todo.txt\n");
	todo_file = fopen("todo.txt", "r");
	if(!todo_file) exit(EXIT_FAILURE);

	while(fgets(buffer, 1000, todo_file) != NULL) printf("%s", buffer);
	fclose(todo_file);

	exit(EXIT_SUCCESS);
}

void todo_remove(int line)
{
	FILE *todo_file;
	FILE *tmp;
	char buffer[1000];
	int count = 0;
	int i = 0;

	printf("[TODOCMD]: Deleting Todo: %d\n", line);
	todo_file = fopen("todo.txt", "r");
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
	todo_file = fopen("todo.txt", "w");
	tmp = fopen("tmp.txt", "r");

	while(fgets(buffer, 1000, tmp) != NULL) {
		fprintf(todo_file, "%s", buffer);
	}

	exit(EXIT_SUCCESS);
}

void todo_help()
{
	printf("todocmd help:\n");
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	if(argc > 1) {
		parse_input(argc, argv);
	} else {
		printf("No Input Recieved. Shutting Down.\n");
	}

        exit(EXIT_SUCCESS);
}
