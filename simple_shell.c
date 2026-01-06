#include "simple_shell.h"

/**
 * main - Entry point for simple shell
 * @argc: argument count
 * @argv: argument vector
 * @envp: environment variables
 * Return: 0 on success
 */
int main(int argc, char **argv, char **envp)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char **args;
	int interactive, cmd_count = 0;
	(void)argc;
	interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
			write(STDOUT_FILENO, "$ ", 2);
		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			if (interactive)
				write(STDOUT_FILENO, "\n", 1);
			break;
		}
		cmd_count++;
		if (line[read - 1] == '\n')
			line[read - 1] = '\0';
		if (line[0] == '\0')
			continue;
		args = parse_input(line);
		if (args == NULL || args[0] == NULL)
		{
			free_argv(args);
			continue;
		}
		if (strcmp(args[0], "exit") == 0)
		{
			free_argv(args);
			break;
		}
		execute_command(args, argv[0], cmd_count, envp);
		free_argv(args);
	}
	free(line);
	return (0);
}
