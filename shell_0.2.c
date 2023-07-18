#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#define MAX_LENGTH 1024
#define MAX_ARGS 64
/**
 * parse_input -  takes the user input as a string and splits
 * it into individual arguments
 * @input: A pointer to a character array
 * @args: arguments
 */
void parse_input(char *input, char **args)
{
	int i = 0;
	const char delimiters[] = " \t\r\n\a";
	char *token = strtok(input, delimiters);

	while (token != NULL)
	{
		args[i++] = token;
		token = strtok(NULL, delimiters);
	}
	args[i] = NULL;
}
/**
 * execute_command - executes the command in a child process
 * @args: arguments
 */
void execute_command(char **args)
{
	pid_t pid;
	int status;

	pid = fork();

	if (pid < 0)
	{
		_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		execve(args[0], args, NULL);
		_exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
	}
}
/**
 * command_exists - checks if a given command exists in any
 * of the directories listed
 * @command: A pointer to a character array
 * @path: A pointer to a character array
 * Return: 0
 */
int command_exists(char *command, char *path)
{
	char *dir;
	char *path_copy = strdup(path);
	char *token = strtok(path_copy, ":");

	while (token != NULL)
	{
		dir = token;
		token = strtok(NULL, ":");
		char *full_path = malloc(strlen(dir) + strlen(command) + 2);
		int i = 0;

		for (; *dir; ++dir, ++i)
			full_path[i] = *dir;
		full_path[i++] = '/';
		for (; *command; ++command, ++i)
			full_path[i] = *command;
		full_path[i] = '\0';

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			free(full_path);
			return (1);
		}
		free(full_path);
	}
	free(path_copy);
	return (0);
}
/**
 * main - checks if the command exists
 * Return: 0
 */
int main(void)
{
	char input[MAX_LENGTH];
	char *args[MAX_ARGS];
	char *path = getenv("PATH");

	while (1)
	{
		write(STDOUT_FILENO, "SimpleShell$ ", 13);

		if (read(STDIN_FILENO, input, sizeof(input)) == -1)
		{
			break;
		}
		input[strcspn(input, "\n")] = '\0';
		parse_input(input, args);

		if (args[0] != NULL && command_exists(args[0], path))
		{
			execute_command(args);
		}
		else
		{
			write(STDOUT_FILENO, "Command not found: ", 19);
			write(STDOUT_FILENO, args[0], strlen(args[0]));
			write(STDOUT_FILENO, "\n", 1);
		}
	}
	return (0);
}
