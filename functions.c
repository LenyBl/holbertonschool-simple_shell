#include "simple_shell.h"

/**
 * *get_path - Retrieves the PATH environment variable
 * @env: Array of environment variables
 *
 * This function searches the environment variables for PATH
 * and returns a pointer to its value (excluding "PATH=").
 * If PATH is not found or env is NULL, it returns NULL.
 *
 * Return: Pointer to the PATH value if found, or NULL if PATH is not found
 * or if env is NULL.
 */
char *get_path(char **env)
{
	int i = 0;

	if (env == NULL)
		return (NULL);
	while (env[i])
	{
		if (strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

/**
 * find_command - Finds the full path of a command
 * @command: Command name to search for
 * @env: Array of environment variables
 *
 * This function checks if the command is an absolute or relative path.
 * If not, it searches through the directories listed in PATH to find
 * an executable file matching the command.
 *
 * Return: A dynamically allocated string containing the full path
 * to the command if found, or NULL otherwise.
 */
char *find_command(char *command, char **env)
{
	char *path, *path_copy, *dir, *full_path;
	struct stat st;
	size_t cmd_len, dir_len;

	if (command == NULL || command[0] == '\0')
		return (NULL);
	if (strchr(command, '/'))
	{
		if (stat(command, &st) == 0 && access(command, X_OK) == 0)
			return (strdup(command));
		return (NULL);
	}
	path = get_path(env);
	if (path == NULL)
		return (NULL);
	path_copy = strdup(path);
	if (path_copy == NULL)
		return (NULL);
	cmd_len = strlen(command);
	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		dir_len = strlen(dir);
		full_path = malloc(dir_len + cmd_len + 2);
		if (full_path == NULL)
		{
			free(path_copy);
			return (NULL);
		}
		sprintf(full_path, "%s/%s", dir, command);
		if (stat(full_path, &st) == 0 && access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

/**
 * execute_command - Executes a command using fork and execve
 * @argv: Array of arguments (command and its options)
 * @prog_name: Name of the shell program
 * @cmd_count: Command counter (used for error messages)
 * @env: Array of environment variables
 *
 * This function finds the command path, creates a child process,
 * and executes the command. The parent process waits for the child
 * to finish and returns the command's exit status.
 *
 * Return: Exit status of the command, or an error code on failure.
 */
int execute_command(char **argv, char *prog_name, int cmd_count, char **env)
{
	pid_t pid;
	int status;
	char *cmd_path;

	if (argv == NULL || argv[0] == NULL)
		return (-1);
	cmd_path = find_command(argv[0], env);
	if (cmd_path == NULL)
	{
		fprintf(stderr, "%s: %d: %s: not found\n",
			prog_name, cmd_count, argv[0]);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		return (-1);
	}
	if (pid == 0)
	{
		if (execve(cmd_path, argv, env) == -1)
		{
			perror(prog_name);
			free(cmd_path);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&status);
		free(cmd_path);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}
	return (0);
}

/**
 * parse_input - Splits a line of input into arguments
 * @line: Input string to parse
 *
 * This function tokenizes the input line using spaces, tabs,
 * and newlines as delimiters, and stores the tokens in a
 * dynamically allocated array.
 *
 * Return: A NULL-terminated array of strings (argv),
 * or NULL on failure.
 */
char **parse_input(char *line)
{
	char **argv;
	char *token;
	int i = 0, bufsize = 64;

	argv = malloc(bufsize * sizeof(char *));
	if (argv == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	token = strtok(line, " \t\n");
	while (token != NULL)
	{
		argv[i] = strdup(token);
		if (argv[i] == NULL)
		{
			perror("malloc");
			while (i > 0)
				free(argv[--i]);
			free(argv);
			return (NULL);
		}
		i++;
		if (i >= bufsize)
		{
			bufsize += 64;
			argv = realloc(argv, bufsize * sizeof(char *));
			if (argv == NULL)
			{
				perror("realloc");
				return (NULL);
			}
		}
		token = strtok(NULL, " \t\n");
	}
	argv[i] = NULL;
	return (argv);
}

/**
 * free_argv - Frees a NULL-terminated array of strings
 * @argv: Array of arguments to free
 *
 * This function releases all memory allocated for the argument
 * array, including each string and the array itself.
 */
void free_argv(char **argv)
{
	int i = 0;

	if (argv == NULL)
		return;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}
