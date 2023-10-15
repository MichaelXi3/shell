CC = gcc
SOURCES = my_shell.c read_command.c parse_command.c execute_command.c builtin_functions.c
OUTPUT = my_shell

build:
	@$(CC) -o $(OUTPUT) $(SOURCES) 

run: build
	@./$(OUTPUT) 

clean:
	@rm -f $(OUTPUT)
