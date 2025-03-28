OBJDIR=.obj
BINDIR=bin
SRCDIR=src
CFLAGS=-c -Wall -Werror -pedantic -g -D_GNU_SOURCE -Isrc
LDFLAGS=-lncurses -lm

SRC_FILES=$(shell find src -type f -name "*.cpp" -o -name "*.c")
SRC_OBJS=$(SRC_FILES:%=$(OBJDIR)/%.o)

MAIN_FILE=main.cpp
MAIN_OBJ=$(OBJDIR)/$(MAIN_FILE).o

all: main

main: $(BINDIR)/main
test: $(BINDIR)/test

$(BINDIR)/main: $(MAIN_OBJ) $(SRC_OBJS)
	mkdir -p $(dir $@)
	g++ $^ $(LDFLAGS) -o $@

$(BINDIR)/test: $(OBJDIR)/tests/test.cpp.o $(SRC_OBJS)
	mkdir -p $(dir $@)
	g++ $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	gcc -std=c99 $(CFLAGS) $< -o $@

$(OBJDIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	g++ -std=c++11 $(CFLAGS) $< -o $@

clean:
	/bin/rm -rf $(OBJDIR) $(BINDIR) vgcore*

.PHONY: clean main test