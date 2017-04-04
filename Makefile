CC = gcc
CFLAGS = -g -Wall -Wextra -Wfatal-errors -pedantic-errors
LDFLAGS =
INC=-I./include
VPATH=src/
OBJDIR=obj/
BINDIR=build/
LIBDIR=lib/

EXEC = \
	   test\
       imresize\

OBJ = \
      frameio.o\
      imtools.o\

EXECS=$(addprefix $(BINDIR), $(EXEC))
OBJS=$(addprefix $(OBJDIR), $(OBJ))
DEPS=$(wildcard include/*.h) Makefile

all: build obj lib $(EXECS)

$(OBJDIR)%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(BINDIR)test: src/test.c $(OBJS)
	$(CC) $(CFLAGS) $(INC) $^ -o $@ $(LDFLAGS)

$(BINDIR)imresize: src/imresize.c $(OBJS)
	$(CC) $(CFLAGS) $(INC) $^ -o $@ $(LDFLAGS)

obj:
	mkdir -p $(OBJDIR)

build:
	mkdir -p $(BINDIR)

lib:
	mkdir -p $(LIBDIR)

.PHONY: clean
clean:
	rm -rf *.o $(BINDIR) $(OBJDIR) $(LIBDIR) core.*
