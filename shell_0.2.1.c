#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>

/**
 * print_prompt -  displays the shell prompt to the user
 */
void print_prompt(void)
{
	write(STDOUT_FILENO, "simpleshell> ", 13);
}
/**
 * read_command - reads the user's input from the standard input
 * and stores it in the command buffer
 * @command: a character array
 */
void read_command(char *command)
{
	print_prompt(void);
	ssize_t bytes_read = read(STDIN_FILENO, command, MAX_COMMAND_LENGTH);

	if (bytes_read > 0 && command[bytes_read - 1] == '\n')
	{
		command[bytes_read - 1] = '\0';
	}
}
/**
 * parse_command - takes the command string, and tokenizes
 * it into individual arguments (tokens)
 * @command: a character array
 * @args: array of character pointers
 */
void parse_command(char *command, char **args)
{
	int i = 0;

	while (*command != '\0')
	{
		while (*command == ' ' || *command == '\t')
		{
			*command++ = '\0';
		}

		args[i++] = command;

		while (*command != '\0' && *command != ' ' & *command != '\t')
		{
			command++;
		}
	}
	args[i] = NULL;
}
/**
 * execute_command - executing the command specified by the user
 * @args: array of character pointers
 * Return: 1
 */
int execute_command(char **args)
{
	pid_t pid;
	int status;

	pid = fork();

	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		if (execve(args[0], args, NULL) == -1)
		{
			perror("execve");
			_exit(1);
		}

	}
	else
	{
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return (1);
}
/**
 * main - the entry point of the program
 * Return: 0
 */
int main(void)
{
	char *command = (char *)malloc(MAX_COMMAND_LENGTH);
	char *args[MAX_ARGUMENTS];

	while (1)
	{
		read_command(command);
		parse_command(command, args);

		if (strcmp(args[0], "exit") == 0)
		{
			free(command);
			break;
		}
		else if (args[0][0] != '\0')
		{
			execute_command(args);
		}
	}
	return (0);
}
