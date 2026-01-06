#include "simple_shell.h"

/**
 * print_env - Prints all environment variables
 * @env: Array of strings containing the environment variables
 *
 * This function displays each environment variable on a new line.
 * It first checks if the environment array is NULL to avoid errors.
 * Then it iterates through the array until a NULL pointer is reached.
 */
void print_env(char **env)
{
	int i = 0;

	if (env == NULL)
		return;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}
