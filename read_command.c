#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_command.h"
#include "builtin_functions.h"

char *read_line() {
    int buffer_size = RL_BUFFER_SIZE;
    char *line = malloc(buffer_size * sizeof(char));
    char *ePtr = NULL;

    if (!line) {
        perror("shell: malloc error\n");
        exit(1);
    }

    if (fgets(line, buffer_size - 1, stdin) != NULL) {
        /* find \n and replace with \0 */
        /* strchr returns the address of \n if found in line */
        ePtr = strchr(line, '\n');
        if (ePtr != NULL) {
            *ePtr = '\0';
        }
        
        /* print the user entered command 
        printf("read_line: [%s] \n", line); */

        /* add to history file */
        sh_history_add_command(line);
    }
    return line;
}