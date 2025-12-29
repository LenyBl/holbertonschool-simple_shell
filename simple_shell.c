#include "simple_shell.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
/**
 * trim_spaces - remove leading and trailing spaces in-place
 * @str: input buffer
 *
 * Return: same buffer, trimmed
 */
char *trim_spaces(char *str)
{
char *start = str;
char *end;
while (*start == ' ' || *start == '\t')
start++;
if (*start == '\0')
{
*str = '\0';
return (str);
}
end = start + strlen(start) - 1;
while (end > start && (*end == ' ' || *end == '\t'))
end--;
*(end + 1) = '\0';
if (start != str)
memmove(str, start, strlen(start) + 1);
return (str);
}
/**
 * main - simple UNIX shell
 *
 * Return: 0
 */
int main(void)
{
char *line = NULL;
size_t len = 0;
ssize_t read;
pid_t pid;
int status;
char *argv[100];
int i;
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
{
i++;
argv[i] = strtok(NULL, " \t");
}
pid = fork();
if (pid == 0)
{
if (execve(argv[0], argv, environ) == -1)
{
fprintf(stderr, "./hsh: %s: No such file or directory\n", argv[0]);
_exit(1);
}
}
else if (pid > 0)
{
wait(&status);
}
else
{
perror("fork");
}
}
free(line);
return (0);
}