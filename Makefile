CC = gcc
CFLAGS = -g -Wall -Werror -Wextra -Wfatal-errors -pedantic-errors
LDFLAGS =

OBJ = \
      frameio.o\
      imtools.o\

EXEC = \
	   main\
       imresize\

all: $(EXEC)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

main: main.c $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

imresize: imresize.c $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


.PHONY: clean
clean:
	rm -rf *.o $(EXEC) core.*
