#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "execute_command.h"
#include "builtin_functions.h"


int execute_line(char ***args, int num_commands) {
    pid_t pid, cpid;
    pid_t children[num_commands];    /* for storing piped commands executing child pid */
    int status = 0;
    int background = 0;
    int i = 0;                       /* for & check loop */
    int j = 0;                       /* for piped commands loop */
    int fd[2 * (num_commands - 1 )];

    /* check for empty command */
    if (args == NULL || args[0] == NULL || args[0][0] == NULL || args[0][0][0] == '\0') {
        return 1;
    }

    char *command_name = args[0][0];

    /* check for built in command */
    if (strcmp(args[0][0], "help") == 0) {
        return sh_help(args);
    }
    else if (strcmp(args[0][0], "exit") == 0) {
        return sh_exit(args);
    }
    else if (strcmp(args[0][0], "history") == 0) {
        if (args[0][1] != NULL && atoi(args[0][1]) > 0) {
            sh_history_display(atoi(args[0][1]));
        } else {
            sh_history();
        }
        return 1;
    }
    else if (strcmp(args[0][0], "erase") == 0 && strcmp(args[0][1], "history") ==0) {
        sh_history_erase();
        return 1;
    }

    /* check for background execution */
    while (args[num_commands - 1][i] != NULL) {
        i++;
    }
    if (i > 0 && strcmp(args[num_commands - 1][i - 1], "&") == 0) {
        background = 1;
        args[num_commands - 1][i - 1] = NULL;
        printf("background process identified \n");
    }

    /* ! if only one command received */
    if (num_commands == 1) {
        pid = fork();
        if (pid < 0) {
            perror("error in fork");
            exit(-1);
        }
        /* child process */
        if (pid == 0) {
            if (execvp(command_name, args[0]) == -1) {
                switch(errno) {
                    case ENOENT:
                        perror("Command not found");
                        break;
                    case EACCES:
                        perror("Permission denied");
                        break;
                    default:
                        perror("execvp error");
                        break;
                }
                exit(-1);
            }
        /* parent process */
        } else {
            if (!background) {
                cpid = waitpid(pid, &status, WUNTRACED);
            } else {
                printf("background process with PID: %d\n", pid);
            }
        }
    /* ! prcess piped commands*/
    } else {
        /* create pipes */
        for (int t = 0; t < num_commands - 1; t++) {
            if (pipe(fd + 2 * t) < 0) {
                perror("create pipe failed\n");
                exit(-1);
            }
        }

        /* fork children to execute multiple piped commands */
        for (int j = 0; j < num_commands; j++) {
            pid = fork();
            if (pid < 0) {
                perror("error in fork\n");
                exit(-1);
            }
            /* child process */
            if (pid == 0) {
                /* if not the first command, redirect stdin from previous pipe */
                if (j != 0) {
                    if (dup2(fd[(j - 1) * 2], STDIN_FILENO) < 0) {
                        perror("dup2 error\n");
                        exit(-1);
                    }
                }
                /* if not the last command, redirect stdout to current pipe */
                if (j != num_commands - 1) {
                    if (dup2(fd[j * 2 + 1], STDOUT_FILENO) < 0) {
                        perror("dup2 error\n");
                        exit(-1);
                    }
                }
                /* close unused pipes in child */
                for (int k = 0; k < 2 * (num_commands - 1); k++) {
                    close(fd[k]);
                }
                /* exec the commands in each child */
                if (execvp(args[j][0], args[j]) < 0) {
                    perror(args[j][0]);
                    exit(-1);
                }
            } else {
                children[j] = pid;
            }
        }

        /* close all pipes in parent */
        for (int l = 0; l < 2 * (num_commands - 1); l++) {
            close(fd[l]);
        }

        /* if not background process */
        if (!background) {
            for (int u = 0; u < num_commands; u++) {
                waitpid(children[u], &status, 0);
            }
        }
    }

    return 1;
}
