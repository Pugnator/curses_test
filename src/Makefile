
CC:=gcc

SRC=$(wildcard *.c)

DEPS=libxml-2.0 lua5.2 ncurses panel menu form

CFLAGS+=-O0 -g3 -Wall -export-dynamic -std=c11 -I./ $(shell pkg-config $(DEPS) --cflags)

LDFLAGS:=$(shell pkg-config $(DEPS) --libs)

OBJ=$(SRC:%.c=%.o)

EXEC:=../bin/xterm

all: $(EXEC)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o $(EXEC)
