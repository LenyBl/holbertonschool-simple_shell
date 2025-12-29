#include "simple_shell.h"
#include <string.h>
/**
 * trim_spaces - remove leading and trailing spaces
 * @str: input string
 *
 * Return: pointer to trimmed string
 */
char *trim_spaces(char *str)
{
char *end;
while (*str == ' ' || *str == '\t')
str++;
if (*str == '\0')
return (str);
end = str + strlen(str) - 1;
while (end > str && (*end == ' ' || *end == '\t'))
end--;
*(end + 1) = '\0';
return (str);
}
/**
 * main - simple UNIX command interpreter
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
line = trim_spaces(line);
if (*line == '\0')
continue;
pid = fork();
if (pid == 0)
{
char *argv[2];
argv[0] = line;
argv[1] = NULL;
if (execve(line, argv, environ) == -1)
{
perror("./hsh");
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
}
