CC = gcc
CFLAGS = -g -Wall -Wextra -Wfatal-errors -pedantic-errors -std=gnu99
LDFLAGS = -lm
INC=-I./include
VPATH=src/
OBJDIR=obj/
BINDIR=bin/
TESTDIR=test/

EXEC = \
	   run_tests\
       imresize\

OBJ = \
      frameio.o\
      imtools.o\
      draw.o\
      image.o\

EXECS=$(addprefix $(BINDIR), $(EXEC))
OBJS=$(addprefix $(OBJDIR), $(OBJ))
DEPS=$(wildcard include/*.h) Makefile

all: bin obj $(EXECS)

$(OBJDIR)%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(BINDIR)run_tests: $(TESTDIR)test.c $(OBJS)
	$(CC) $(CFLAGS) $(INC) $^ -o $@ $(LDFLAGS)

$(BINDIR)imresize: src/imresize.c $(OBJS)
	$(CC) $(CFLAGS) $(INC) $^ -o $@ $(LDFLAGS)

obj:
	mkdir -p $(OBJDIR)

bin:
	mkdir -p $(BINDIR)

.PHONY: clean
clean:
	rm -rf *.o python/*.so python/*.pyc python/__pycache__ $(BINDIR) $(OBJDIR) core.* *.mp4 *.jpg *.png
