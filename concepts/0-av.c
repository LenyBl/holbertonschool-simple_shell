#include <stdio.h>
#include <stdarg.h>
/**
 * main - Entry point for testing va_list functionality
 *
 * @argc Argument count
 * @argv Argument vector
 *
 * Return: Always 0 (Success)
 */
int main(int ac, char **av)
{
	int count = 0;

	(void)ac;

	while (av[count] != NULL)
	{
		printf("Argument %d: %s\n", count, av[count]);
		count++;
	}

	return (0);
}
