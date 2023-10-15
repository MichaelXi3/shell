# Shell Description

Hello!!! Welcome to Michael's Shell.
- Type `exit` to exit the shell 
- Type command names and args to execute the command 
- Use `|` to pipe as many commands as you want 
- To erase history, type `erase history`
- To check recent history, type `history 4` (or any number) 
- The size of history file is set in default 1000 or by env_var `MAX_HISTORY`
- Built functions: `help`, `exit`, `history`

# Build Instructions

```
make build
```

```
make run
```

```
make clean
```

# Project Structure

```
├── [ 211]  Makefile
├── [4.5K]  builtin_functions.c
├── [ 509]  builtin_functions.h
├── [4.5K]  execute_command.c
├── [ 110]  execute_command.h
├── [ 22K]  my_shell*
├── [1.0K]  my_shell.c
├── [1.7K]  parse_command.c
├── [ 185]  parse_command.h
├── [ 790]  read_command.c
└── [ 103]  read_command.h
```