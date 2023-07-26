#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>

/**
 * execute_command -  executing a single command
 * @command: a pointer to a character array
 */
void execute_command(char *command)
{
	char *arguments[MAX_ARGUMENTS], *token = strtok(command, " \t\n");
	int i = 0;
	char *token = strtok(command, " \t\n");

	while (token && i < MAX_ARGUMENTS - 1)
		arguments[i++] = token, token = strtok(NULL, " \t\n");
	arguments[i] = NULL;

	if (strcmp(arguments[0], "cd") == 0)
	{
		if (i < 2)
			write(STDERR_FILENO, "cd: missing argument\n", 21);
		else
			chdir(arguments[1]) == 0 ?: perror("cd");
		return;
	}
	if (strcmp(arguments[0], "exit") == 0)
	{
		write(STDOUT_FILENO, "exiting the shell...\n", 21);
		_exit(0);
	}
	pid_t pid;
	pid = fork();

	if (pid < 0)
	{
		perror("fork");
		return;
	}
	if (pid == 0)
	{
		execvp(arguments[0], arguments);
		perror("execvp");
		_exit(EXIT_FAILURE);
	}
	waitpid(pid, NULL, 0);
}
/**
 * main -  entry point of the program
 * Return: 0
 */
int main(void)
{
	char command_line[MAX_COMMAND_LENGTH];

	write(STDOUT_FILENO, "simple shell 1.0 +\n", 20);

	while (1)
	{
		write(STDOUT_FILENO, "shell > ", 8);

		ssize_t read_bytes = read(STDIN_FILENO, command_line, MAX_COMMAND_LENGTH);

		if (read_bytes <= 0)
			break;

		command_line[read_bytes] = '\0';

		char *token = strtok(command_line, ";");

		while (token)
		{
			execute_command(token);
			token = strtok(NULL, ";");
		}
	}
	return (0);
}
