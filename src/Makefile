CC=gcc
CFLAGS=-O2 -Wall -Wextra -Wpedantic -Werror -g 
SFLAGS= -fsanitize=leak -fsanitize=address -fsanitize=undefined 
.PHONY: all
all: main
main: main.c crypto.c util.c test.c
	$(CC) $(CFLAGS) -o $@ $^ 

.PHONY: clean	 
clean:
	rm -rf main

# compile with sanitizer flags
.PHONY: sanitize
sanitize: main.c crypto.c util.c test.c
	make clean
	$(CC) $(CFLAGS) $(SFLAGS) -o main $^
