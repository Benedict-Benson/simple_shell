#ifndef SHELL_H
#define SHELL_H

#define MAX_LENGTH 1024
#define MAX_ARGS 64

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 64

#define MAX_ALIASES 100
#define MAX_ALIAS_NAME 50
#define MAX_ALIAS_VALUE 100

/*structure to store aliases*/
struct Alias
{
    char name[MAX_ALIAS_NAME];
    char value[MAX_ALIAS_VALUE];
};

void print_aliases(const struct Alias* aliases, int num_aliases);
void set_alias(struct Alias* aliases, int *num_aliases, const char *name, const char *value);
void parse_alias_command(const char *command, struct Alias* aliases, int *num_aliases);
void write_string(const char *str, int out_fd);
void execute_command(char *command);
int main(void);
void executeCommand(const char *command);
void prompt(void);
void write_to_stderr(const char *message);
void handle_setenv(char *args[]);
void handle_unsetenv(char *args[]);
void process_command(char *command);
void print_prompt(void);
void read_command(char *command);
void parse_command(char *command, char **args);
void parse_input(char *input, char **args);
void execute_command(char **args);
int command_exists(char *command, char *path);
int count_strings(char **array);
void print_string(char *str);
void print_integer(int num);
void display_environment(char **env);

#endif
