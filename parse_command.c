#include "parse_command.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char ***parse_line(char *line, int *num_commands) {
    int buffer_size = TK_BUFFER_SIZE;
    int index = 0;
    char *token;
    char **commands = malloc(buffer_size * sizeof(char*));
    char ***tokens;

    /* check for empty command */
    if (line == NULL || line[0] == '\0') {
        *num_commands = 0;
        return NULL;
    }

    if (!commands) {
        perror("shell: commands malloc error\n");
        exit(1);
    }
    
    /* separate the input line by pipe if any */
    token  = strtok(line, TK_DELIM_PIPE);
    while (token != NULL) {
        commands[index] = token;
        index++;
        /* subsequent calls to strtok should have the first parameter 
        as NULL to tokenize the same string from where it left off */
        token = strtok(NULL, TK_DELIM_PIPE);
    }
    commands[index] = NULL;
    *num_commands = index;


    /* start to parse each command */
    tokens = malloc(index * sizeof(char**));
    if (!tokens) {
        perror("shell: tokens malloc error\n");
        exit(1);
    }
    for (int i = 0; i < index; i++) {
        int args_index = 0;
        tokens[i] = malloc(buffer_size * sizeof(char*));
        if (!tokens[i]) {
            perror("shell: tokens[i] malloc error\n");
            exit(1);
        }

        token = strtok(commands[i], TK_DELIM);
        while (token != NULL) {
            tokens[i][args_index] = token;
            printf("parse command %d: [%s] \n", i, token);
            args_index++;
            token = strtok(NULL, TK_DELIM);
        }
        tokens[i][args_index] = NULL;
    }
    free(commands);
    return tokens;
}