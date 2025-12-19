#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/**
 *
 * _getline - Reads a line from standard input
 *
 * @lineptr: Pointer to buffer where the line will be stored
 * @n: Pointer to size of the buffer
 * @stream: Input stream to read from
 *
 * Return: Pointer to the read line, or NULL on failure
 */
ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
	int c;
	size_t i = 0;
	char *tmp;

	if (!lineptr || !n || !stream)
		return -1;

	if (*lineptr == NULL || *n == 0)
	{
		*n = 128;
		*lineptr = malloc(*n);
		if (*lineptr == NULL)
			return -1;
	}

	while ((c = fgetc(stream)) != EOF)
	{
		if (i + 1 >= *n)
		{
			*n *= 2;
			tmp = realloc(*lineptr, *n);
			if (!tmp)
				return -1;
			*lineptr = tmp;
		}

		(*lineptr)[i++] = c;

		if (c == '\n')
			break;
	}

	if (i == 0 && c == EOF)
		return -1;

	(*lineptr)[i] = '\0';
	return i;
}
/**
 * main - Entry point for reading and printing lines from standard input
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	return 0;
}