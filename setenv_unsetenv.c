#include "shell.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void write_to_stderr(const char *message)
{
    write(STDERR_FILENO, message, strlen(message));
}

void handle_setenv(char *args[])
{
	if (args[1] && args[2])
	{
		int overwrite = 1;
		
		if (setenv(args[1], args[2], overwrite) != 0)
		{
			write_to_stderr("setenv: Failed to set environment variable\n");
		}
	}
	else
	{
		write_to_stderr("setenv: Missing variable or value\n");
	}
}

void handle_unsetenv(char *args[])
{
	if (args[1])
	{
		if (unsetenv(args[1]) != 0)
		{
			write_to_stderr("unsetenv: Failed to unset environment variable\n");
		}
	}
	else
	{
		write_to_stderr("unsetenv: Missing variable\n");
	}
}

void process_command(char *command)
{
	char *args[4] = { NULL }, *token = strtok(command, " \n");
	int i = 0;
	
	while (token != NULL && i < 3)
	{
		args[i++] = token, token = strtok(NULL, " \n");
	}
	args[i] = NULL;
	
	if (i == 0)
	{
		return;
	}
	
	if (strcmp(args[0], "setenv") == 0)
	{
		handle_setenv(args);
	}
	else if (strcmp(args[0], "unsetenv") == 0)
	{
		handle_unsetenv(args);
	}
	else
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
}

int main(void)
{
	while (1)
	{
		write_to_stderr("Your shell prompt $ ");
		char command[256];
		
		if (fgets(command, sizeof(command), stdin) == NULL)
		{
			break;
		}
		process_command(command);
	}
	return (0);
}
