#ifndef PARSE_COMMAND_H
#define PARSE_COMMAND_H

#define TK_BUFFER_SIZE 128
#define TK_DELIM " \t\n"
#define TK_DELIM_PIPE "|"

char ***parse_line(char *line, int *num_command);

#endif