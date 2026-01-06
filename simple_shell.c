#include "simple_shell.h"

/**
 * main - Entry point of the simple shell program
 * @argc: Argument count (unused)
 * @argv: Argument vector containing the program name
 * @envp: Array of environment variables
 *
 * This function implements a simple command-line shell.
 * It displays a prompt in interactive mode, reads user input,
 * parses commands, and executes them.
 *
 * Built-in commands handled:
 * - exit: exits the shell
 * - env: prints the environment variables
 *
 * The shell continues running until EOF is reached or the
 * exit command is entered.
 *
 * Return: The exit status of the last executed command.
 */
int main(int argc, char **argv, char **envp)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char **args;
	int interactive, cmd_count = 0, status = 0;
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
		if (strcmp(args[0], "env") == 0)
		{
			print_env(envp);
			free_argv(args);
			continue;
		}
		status = execute_command(args, argv[0], cmd_count, envp);
		free_argv(args);
	}
	free(line);
	return (status);
}
