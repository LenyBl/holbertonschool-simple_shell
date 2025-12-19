#include <stdio.h>
#include <unistd.h>
#include <sched.h>
/**
 * getpidmax - Retrieves the maximum number of processes
 *               that a user can have
 *
 * Return: Maximum number of processes, or -1 on failure
 */
pid_t getpidmax(void)
{
	FILE *file;
	pid_t max_procs;

	file = fopen("/proc/sys/kernel/pid_max", "r");
	if (file == NULL)
	{
		perror("Failed to open /proc/sys/kernel/pid_max");
		return -1;
	}

	if (fscanf(file, "%d", &max_procs) != 1)
	{
		perror("Failed to read max process ID");
		fclose(file);
		return -1;
	}

	fclose(file);
	return max_procs;
}
/**
 * main - Entry point to get and print the maximum number of processes
 * 	  that a user can have
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	pid_t max_procs;

	max_procs = getpidmax();

	if (max_procs != -1)
	{
		printf("Maximum Process ID: %d\n", max_procs);
		return (0);
	} else
	{
		return (1);
	}
}
