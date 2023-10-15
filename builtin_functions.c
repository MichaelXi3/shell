#include "builtin_functions.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


char *builtin_function[] = {
    "help",
    "exit",
    "history"
};

int sh_help(char ***args) {
    printf(" /\\ /\\\n");
    printf("( o.o )\n");
    printf(" > ^ < \n");
    printf("Hello!!! Welcome to Michael's Shell.\n");
    printf("----------------------------------------------------- \n");
    printf(" - Type 'exit' to exit the shell \n");
    printf(" - Type command names and args to execute the command \n");
    printf(" - Use '|' to pipe as many commands as you want \n");
    printf(" - To erase history, type 'erase history' \n");
    printf(" - To check recent history, type 'history 4 (or any number)' \n");
    printf(" - The size of history file is set in default 1000 or by env_var MAX_HISTORY' \n");
    printf(" - Built-in functions: \n");
    for (int i = 0; i < NUM_BUILT_IN; i++) {
        printf("    %s\n", builtin_function[i]);
    }
    printf("Hope you enjoy my shell! \n");
}

int sh_exit(char ***args) {
    return 0;
}

void sh_history() {
    FILE *fp;
    int line_number = 1;
    char line[520];

    /* open the history file */
    fp = fopen(HISTORY_FILE_PATH, "r");
    if (fp == NULL) {
        perror("Failed open history file\n");
        exit(-1);
    }

    /* iterate and print the file content */
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%d %s", line_number, line);
        line_number++;
    }
    printf("---------------------------\n");
    printf("Total history commands: %d\n", line_number - 1);
}

void sh_history_add_command(char *command) {
    int count_of_history_lines = sh_history_count();
    int MAX_HISTORY_LIMIT = sh_history_limit();
    if (count_of_history_lines >= MAX_HISTORY_LIMIT) {
        sh_history_trim();
    }
    sh_history_append(command);
}

void sh_history_append(char *command) {
    /* FILE mode 'a' means append, only write */
    FILE *fp = fopen(HISTORY_FILE_PATH, "a");
    if (fp) {
        fprintf(fp, "%s\n", command);
        fclose(fp);
    }
}

void sh_history_trim() {
    /* open history file as rw */
    FILE *fp = fopen(HISTORY_FILE_PATH, "r+");
    if (fp == NULL) {
        perror("history FILE open failed\n");
        exit(-1);
    }
    /* count the history file total line count */
    int line_count = sh_history_count();

    /* to avoid too much trims, trim up to 3/4 of the limit each time */
    int limit = sh_history_limit();
    int lines_to_trim = (line_count - limit) + limit / 4;

    /* locate the fp position to retain */
    char c;
    rewind(fp);
    while (lines_to_trim > 0) {
        c = getc(fp);
        if (c == '\n') {
            lines_to_trim -= 1;
        }
    }

    /* create tmp file to keep the content to retain */
    FILE *tmp = tmpfile();
    if (tmp == NULL) {
        perror("tmp FILE created failed\n");
        exit(-1);
    }
    while ((c = getc(fp)) != EOF) {
        putc(c, tmp);
    }

    /* erase the previous full history file */
    fclose(fp);
    fp = fopen(HISTORY_FILE_PATH, "w");
    if (fp == NULL) {
        perror("history FILE open failed\n");
        fclose(tmp);
        exit(-1);
    }

    /* copy the trimmed history file content into new file */
    rewind(tmp);
    while ((c = getc(tmp)) != EOF) {
        putc(c, fp);
    }
    fclose(fp);
    fclose(tmp);
}

void sh_history_display(int n) {
    int line_count = sh_history_count();
    if (n > line_count) {
        sh_history();
        return;
    }
    FILE *fp = fopen(HISTORY_FILE_PATH, "r");
    char line[520];
    int start_pos = line_count - n;
    int line_number = 1;
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (line_number - 1 < start_pos) {
            line_number++;
            continue;
        }
        printf("%d %s", line_number, line);
        line_number++;
    }
}

void sh_history_erase() {
    FILE *fp = fopen(HISTORY_FILE_PATH, "w");
    if (!fp) {
        perror("Error opening history file\n");
        return;
    }
    fclose(fp);
}

int sh_history_count() {
    FILE *fp = fopen(HISTORY_FILE_PATH, "r");

    /* if open failed, file not exist yet */
    if (fp == NULL) {
        perror("history file open failed\n");
        return 0;
    }

    int count = 0;
    char ch;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    return count;
}

int sh_history_limit() {
    char *env_max_value = getenv("MAX_HISTORY");
    if (env_max_value) {
        int max_limit = atoi(env_max_value);
        if (max_limit > 0) {
            return max_limit;
        }
    }
    return DEFAULT_MAX_HISTORY;
}

