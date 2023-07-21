#ifndef SHELL_H
#define SHELL_H

#define MAX_ALIASES 100
#define MAX_ALIAS_NAME 50
#define MAX_ALIAS_VALUE 100

/*structure to store aliases*/
struct Alias
{
    char name[MAX_ALIAS_NAME];
    char value[MAX_ALIAS_VALUE];
};

void print_aliases(const struct * aliases, int num_aliases);
void set_alias(struct * aliases, int *num_aliases, const char *name, const char *value);
void parse_alias_command(const char *command, struct * aliases, int *num_aliases);

void write_string(const char *str, int out_fd);

#endif
