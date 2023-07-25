#include "shell.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * write_to_stderr - writes the specified message to the standard error stream
 * @message: written to stderr
 */
void write_to_stderr(const char *message)
{
	write(STDERR_FILENO, message, strlen(message));
}
/**
 * execute_command - executes other shell commands by forking a new process
 * @args: An array of strings 
 */
void execute_command(char *args[])
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("fork");
		return;
	}
	else if (pid == 0)
	{
		execvp(args[0], args);
		perror("execvp");
		_exit(EXIT_FAILURE);
	}
	else
	{
		wait(NULL);
	}
}
/**
 * process_command - processes the user input command by parsing & executing
 * @command: A string containing the user's command input
 */
void process_command(char *command)
{
	char *args[4] = { NULL }, *token = strtok(command, " \n");
	int i = 0, overwrite = 1;

	while (token != NULL && i < 3)
	{
		args[i++] = token;
		token = strtok(NULL, " \n");
	}
	args[i] = NULL;
	if (i == 0)
	{
		return;
	}
	if (strcmp(args[0], "setenv") == 0)
	{
		if (args[1] && args[2])
		{
			if (setenv(args[1], args[2], overwrite) != 0)
			{
				write_to_stderr("setenv: failed to set environment variable\n");
			}
		}
		else
		{
			write_to_stderr("setenv: missing variable or value\n");
		}
	}
	else if (strcmp(args[0], "unsetenv") == 0)
	{
		if (args[1])
		{
			if (unsetenv(args[1]) != 0)
			{
				write_to_stderr("unsetenv: failed to unset environment variable\n");
			}
		}
		else
		{
			write_to_stderr("unsetenv: missing variable\n");
		}
	}
	else
	{
		execute_command(args);
	}
}
/**
 * main -  implements the simple shell
 * 
 */
int main(void)
{
	while (1)
	{
		write_to_stderr("your shell prompt $ ");
		char command[256];

		if (fgets(command, sizeof(command), stdin) == NULL)
		{
			break;
		}
		process_command(command);
	}
	return (0);
}
