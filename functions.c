#include "simple_shell.h"

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