#include "shell.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

/**
 * count_strings -  returns the count of strings
 * @array: a pointer to a null-terminated array
 * Return: count
 */
int count_strings(char **array)
{
	int count = 0;

	while (array[count] != NULL)
	{
		count++;
	}
	return (count);
}
/**
 * print_string - writes the characters of the string
 * @str: a pointer to a null-terminated character array
 */
void print_string(char *str)
{
	write(STDOUT_FILENO, str, strlen(str));
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
	for (int i = len - 1; i >= 0; i--)
	{
		write(STDOUT_FILENO, &buf[i], 1);
	}
}
/**
 * display_environment - takes a pointer to an array of strings
 * @env: a pointer to an array of strings
 */
void display_environment(char **env)
{
	int count = count_strings(env);

	for (int i = 0; i < count; i++)
	{
		print_string(env[i]);
		write(STDOUT_FILENO, "\n", 1);
	}
}
/**
 * main - executes the code within its body when the program is run
 * Return: 0
 */
int main(void)
{
	display_environment(environ);
	return (0);
}
