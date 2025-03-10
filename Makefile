OBJDIR=.obj
BINDIR=bin
SRCDIR=src
CFLAGS=-c -Wall -Werror -pedantic -g -D_GNU_SOURCE
LDFLAGS=-lncurses -lm

COMMON=$(shell find src -type f -name "*.cpp" -o -name "*.c")
COMMON_OBJ=$(COMMON:%=$(OBJDIR)/%.o)

all: bin/editor

$(BINDIR)/editor: $(OBJDIR)/main.cpp.o $(COMMON_OBJ)
	mkdir -p $(dir $@)
	g++ $^ $(LDFLAGS) -o $@

# $(BINDIR)/test_%: $(OBJDIR)/test_%.cpp.o $(COMMON_OBJ)
# 	mkdir -p $(dir $@)
# 	g++ $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	gcc -std=c99 $(CFLAGS) $< -o $@

$(OBJDIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	g++ -std=c++11 $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	/bin/rm -rf $(OBJDIR) $(BINDIR) vgcore* *.log
