#include <stdio.h>
#include <unistd.h>
#include <sched.h>
/**
 * main - Entry point to get and print the parent process ID
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	pid_t ppid;

	ppid = getppid();
	printf("Parent Process ID: %d\n", ppid);

	return 0;
}