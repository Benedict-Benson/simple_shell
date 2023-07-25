#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * prompt - display the shell prompt and handle user commands
 *
 */
void prompt(void)
{
	char command[MAX_COMMAND_LENGTH];

	write(1, "simple shell 0.1\n", 17);

	while (1)
	{
		ssize_t bytesread = read(0, command, MAX_COMMAND_LENGTH - 1);

		if (bytesread == 0)
		{
			write(1, "\ngoodbye!\n", 10);
			break;
		}
		command[bytesread - 1] = '\0';
		execute_command(command);
	}
}
/**
 * execute_command - execute the user's command
 * @command: character array
 */
void execute_command(char *command)
{
	pid_t pid = fork();

	if (pid < 0)
	{
		write(2, "error: fork failed\n", 19);
		_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		char *args[] = {command, NULL};

		if (execve(command, args, NULL) == -1)
		{
			write(2, "error: command not found\n", 24);
			_exit(EXIT_FAILURE);
		}
	}
	else
	{
		int status;

		if (wait(&status) == -1)
		{
			write(2, "error: wait failed\n", 19);
			_exit(EXIT_FAILURE);
		}
	}
}
/**
 * main - returns an integer value
 * Return: 0
 */
int main(void)
{
	prompt();
	return (0);
}
