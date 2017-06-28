CC = gcc
CFLAGS = -g -Wall -Wextra -Wfatal-errors -pedantic-errors
LDFLAGS =
INC=-I./include
VPATH=src/
OBJDIR=obj/
BINDIR=build/
TESTDIR=test/

EXEC = \
	   run_tests\
       imresize\

OBJ = \
      frameio.o\
      imtools.o\

EXECS=$(addprefix $(BINDIR), $(EXEC))
OBJS=$(addprefix $(OBJDIR), $(OBJ))
DEPS=$(wildcard include/*.h) Makefile

all: build obj $(EXECS)

$(OBJDIR)%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(BINDIR)run_tests: $(TESTDIR)test_driver.c $(OBJS)
	$(CC) $(CFLAGS) $(INC) $^ -o $@ $(LDFLAGS)

$(BINDIR)imresize: src/imresize.c $(OBJS)
	$(CC) $(CFLAGS) $(INC) $^ -o $@ $(LDFLAGS)

obj:
	mkdir -p $(OBJDIR)

build:
	mkdir -p $(BINDIR)

.PHONY: clean
clean:
	rm -rf *.o python/*.so $(BINDIR) $(OBJDIR) core.*
