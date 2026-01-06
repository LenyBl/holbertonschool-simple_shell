#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

/* PATH and command search functions */
char *get_path(char **env);
char *find_command(char *command, char **env);

/* Command execution functions */
int execute_command(char **argv, char *prog_name, int cmd_count, char **env);

/* Input parsing functions */
char **parse_input(char *line);

/* Memory management functions */
void free_argv(char **argv);

void print_env(char **env);

#endif /* SIMPLE_SHELL_H */
