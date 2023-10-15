#ifndef BUILT_IN_FUNCTIONS_H
#define BUILT_IN_FUNCTIONS_H

#define NUM_BUILT_IN 3
#define DEFAULT_MAX_HISTORY 1000
#define HISTORY_FILE_PATH ".myhistory"

int sh_help(char ***args);
int sh_exit(char ***args);

/* history command usages */
void sh_history();
void sh_history_display(int n);
void sh_history_erase();

/* history command helpers */
void sh_history_add_command(char *command);
void sh_history_append(char *command);
void sh_history_trim();
int sh_history_count();
int sh_history_limit();

#endif
