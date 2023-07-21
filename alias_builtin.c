#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

/**
 * write_string - write a string to the standard output or standard error
 * @str: A pointer to a constant character array
 * @out_fd: An integer representing the file descriptor
 */
void write_string(const char *str, int out_fd)
{
	size_t len = strlen(str);

	write(out_fd, str, len);
}
/**
 * print_aliases - printing all the aliases stored in the aliases array
 * @aliases: a pointer to a constant array
 * @num_aliases: an integer representing the number of aliases stored
 */
void print_aliases(const struct * aliases, int num_aliases)
{
	for (int i = 0; i < num_aliases; i++)
	{
		char alias_output[MAX_ALIAS_NAME + MAX_ALIAS_VALUE + 6];
		int len = 0;

		for (int j = 0; aliases[i].name[j] != '\0'; j++)
		{
			alias_output[len++] = aliases[i].name[j];
		}
		alias_output[len++] = '=';

		for (int j = 0; aliases[i].value[j] != '\0'; j++)
		{
			alias_output[len++] = aliases[i].value[j];
		}
		alias_output[len++] = '\n';
		alias_output[len] = '\0';
		write(STDOUT_FILENO, alias_output, len);
	}
}
/**
 * set_alias -  is used to set or update an alias
 * @aliases: a pointer to an array
 * @num_aliases: a pointer to an integer
 * @name: A pointer to a constant character array
 * @value: A pointer to a constant character array
 */
void set_alias(struct * aliases, int *num_aliases,
		const char *name, const char *value)
{
	for (int i = 0; i < *num_aliases; i++)
	{
		if (strcmp(aliases[i].name, name) == 0)
		{
			strcpy(aliases[i].value, value);
			return;
		}
	}
	if (*num_aliases < MAX_ALIASES)
	{
		strcpy(aliases[*num_aliases].name, name);
		strcpy(aliases[*num_aliases].value, value);
		(*num_aliases)++;
	}
	else
	{
		write_string("error: maximum number of
				aliases reached\n", STDERR_FILENO);	}
}
/**
 * parse_alias_command - parsing the alias command provided by the user
 * @command: A pointer to a constant character array
 * @aliases: a pointer to an array
 * @num_aliases: a pointer to an integer
 */
void parse_alias_command(const char *command, struct * aliases,
		int *num_aliases)
{
	char *token = strtok((char *)command, " ");

	token = strtok(NULL, " ");

	if (token == NULL)
	{
		print_aliases(aliases, *num_aliases);
	}
	else
	{
		while (token != NULL)
		{
			char *equal_sign = strchr(token, '=');

			if (equal_sign)
			{
				*equal_sign = '\0';
				set_alias(aliases, num_aliases, token, equal_sign + 1);
			}
			else
			{
				print_aliases(aliases, *num_aliases);
			}
			token = strtok(NULL, " ");
		}
	}
}
/**
 * main - entry point of the program
 * Return: 0
 */
int main(void)
{
	struct Alias aliases[MAX_ALIASES];
	int num_aliases = 0;
	char user_input[100];

	while (1)
	{
		write_string("$ ", STDOUT_FILENO);
		ssize_t input_length = read(STDIN_FILENO,
				user_input, sizeof(user_input) - 1);

		if (input_length > 0 && user_input[input_length - 1] == '\n')
		{
			user_input[input_length - 1] = '\0';
		}
		else
		{
			user_input[input_length] = '\0';
		}
		if (strcmp(user_input, "exit") == 0)
		{
			break;
		}
		if (strcmp(user_input, "alias", 5) == 0)
		{
			parse_alias_command(user_input, aliases, &num_aliases);
		}
		else
		{
			char *arguments[] = {"/bin/sh", "-c", user_input, NULL};

			execve(arguments[0], arguments, NULL);
		}
	}
	return (0);
}
