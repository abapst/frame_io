CC = gcc
CFLAGS = -g -Wall
LDFLAGS =

OBJ = frameio.o
EXEC = main

%.o: %.c %.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(EXEC): main.c $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

all: $(EXEC)

.PHONY: clean
clean:
	rm -rf *.o $(EXEC) core.*
