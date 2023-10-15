#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "read_command.h"
#include "parse_command.h"
#include "execute_command.h"

void signal_handler(int signal);

int main() {
    char *input_line;
    char ***argument_list;
    int status;
    int num_commands;

    /* ignore SIGINT */
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("Failed to handle SIGINT \n");
    }

    /* shell begin */
    do {
        printf(">> ");
        /* part 1: read line of command */ 
        input_line = read_line();
        /* part 2: parse line into args */
        argument_list = parse_line(input_line, &num_commands); 
        /* part 3: execute the command */
        status = execute_line(argument_list, num_commands);
        /* part 4: free allocate memory */
        free(input_line);
        for (int i = 0; i < num_commands; i++) {
            free(argument_list[i]);
        }
        free(argument_list);
    } while (status);

    return 0;
}

void signal_handler(int signal) {
    return;
}
