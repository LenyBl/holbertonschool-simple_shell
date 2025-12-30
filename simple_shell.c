#include "simple_shell.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
extern char **environ;
char *trim_spaces(char *str)
{
char *start = str, *end;
while (*start == ' ' || *start == '\t')
start++;
if (*start == '\0')
{
*str = '\0';
return str;
}
end = start + strlen(start) - 1;
while (end > start && (*end == ' ' || *end == '\t'))
end--;
*(end + 1) = '\0';
if (start != str)
memmove(str, start, strlen(start) + 1);
return str;
}
char *get_path(void)
{
int i = 0;
while (environ[i])
{
if (strncmp(environ[i], "PATH=", 5) == 0)
return environ[i] + 5;
i++;
}
return NULL;
}
char *search_path(char *cmd)
{
static char full[1024];
char *path, *copy, *token;
path = get_path();
if (!path || *path == '\0')
return NULL;
copy = strdup(path);
if (!copy)
return NULL;
token = strtok(copy, ":");
while (token)
{
snprintf(full, sizeof(full), "%s/%s", token, cmd);
if (access(full, X_OK) == 0)
{
free(copy);
return full;
}
token = strtok(NULL, ":");
}
free(copy);
return NULL;
}
int main(void)
{
char *line = NULL;
size_t len = 0;
ssize_t read;
char *argv[100], *cmd_path;
pid_t pid;
int status, i;
while (1)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "#cisfun$ ", 9);
read = getline(&line, &len, stdin);
if (read == -1)
{
free(line);
exit(0);
}
if (line[read - 1] == '\n')
line[read - 1] = '\0';
trim_spaces(line);
if (*line == '\0')
continue;
i = 0;
argv[i] = strtok(line, " \t");
while (argv[i] && i < 99)
argv[++i] = strtok(NULL, " \t");
if (strchr(argv[0], '/'))
cmd_path = argv[0];
else
cmd_path = search_path(argv[0]);
if (!cmd_path)
{
fprintf(stderr, "./hsh: %s: No such file or directory\n", argv[0]);
continue;
}
pid = fork();
if (pid == 0)
{
execve(cmd_path, argv, environ);
perror("./hsh");
_exit(1);
}
else if (pid > 0)
wait(&status);
else
perror("fork");
}
free(line);
return 0;
}

