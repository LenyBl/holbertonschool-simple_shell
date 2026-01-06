#include "simple_shell.h"

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