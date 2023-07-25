#include "shell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>

/**
 * executeCommand -  handling the execution of commands entered by the user
 * @command:  A pointer to a string
 */
void executeCommand(const char *command)
{
	char *args[MAX_COMMAND_LENGTH];
	char *token;
	int arg_count = 0;

	token = strtok((char *)command, " ");
	while (token != NULL)
	{
		args[arg_count] = token;
		arg_count++;
		token = strtok(NULL, " ");
	}
	args[arg_count] = NULL;

	if (strcmp(args[0], "exit") == 0)
	{
		_exit(0);
	}
	pid_t pid = fork();

	if (pid < 0)
	{
		char error_message[] = "fork failed\n";

		write(STDERR_FILENO, error_message, strlen(error_message));
		return;
	}
	else if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			char error_message[] = "execution failed\n";

			write(STDERR_FILENO, error_message, strlen(error_message));
			_exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(NULL);
	}
}
/**
 * main - entry point of the program
 * Return: 0
 */
int main(void)
{
	char input[MAX_COMMAND_LENGTH];
	char prompt[] = "> ";

	write(STDOUT_FILENO, "simple shell 1.0 +\n", strlen("simple shell 1.0 +\n"));

	while (1)
	{
		write(STDOUT_FILENO, prompt, strlen(prompt));
		ssize_t read_bytes = read(STDIN_FILENO, input, MAX_COMMAND_LENGTH);

		if (read_bytes > 0 && input[read_bytes - 1] == '\n')
			input[read_bytes - 1] = '\0';

		if (input[0] == '#')
		{
			write(STDOUT_FILENO, "comment: ", strlen("comment: "));
			write(STDOUT_FILENO, input, strlen(input));
			write(STDOUT_FILENO, "\n", 1);
			continue;
		}
		executeCommand(input);
	}
	return (0);
}
