#include "shell.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

/**
 * print_string - writes the characters of the string
 * @str: a pointer to a null-terminated character array
 */
void print_string(char *str)
{
	while (*str != '\0')
	{
		write(STDOUT_FILENO, str, 1);
		str++;
	}
}
/**
 * print_integer - converts the integer into a string representation
 * @num:  integer value
 */
void print_integer(int num)
{
	char buf[20];
	int len = 0;

	if (num == 0)
	{
		print_string("0");
		return;
	}
	if (num < 0)
	{
		write(STDOUT_FILENO, "-", 1);
		num = -num;
	}
	while (num > 0)
	{
		buf[len++] = '0' + (num % 10);
		num /= 10;
	}
	int i;

	for (i = len - 1; i >= 0; i--)
	{
		write(STDOUT_FILENO, &buf[i], 1);
	}
}
/**
 * display_environment - takes a pointer to an array of strings
 * @env: a pointer to an array of strings
 */
void display_environment(char *envp[])
{
	char **env_ptr = envp;

	while (*env_ptr != NULL)
	{
		print_string(*env_ptr);
		write(STDOUT_FILENO, "\n", 1);
		env_ptr++;
	}
}
/**
 * main - executes the code within its body when the program is run
 * @argc: number of command-line arguments
 * @argv: an array of strings
 * @envp: an array of strings
 * Return: 0
 */
int main(int argc, char *argv[], char *envp[])
{
	display_environment(envp);
	return (0);
}
