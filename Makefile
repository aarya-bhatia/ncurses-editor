OBJDIR=.obj
BINDIR=bin
SRCDIR=.
SRC=$(shell ls *.c | grep -v '^test')
OBJS=$(SRC:%.c=$(OBJDIR)/%.o)
LDFLAGS=-lncurses -lm

EDITOR=editor.c vec2.c edit_buffer.c main.c
EDITOR_OBJS=$(EDITOR:%.c=$(OBJDIR)/%.o)

all: $(BINDIR)/editor $(BINDIR)/test_edit_buffer $(BINDIR)/test_vec

$(BINDIR)/editor: $(EDITOR_OBJS)
	mkdir -p $(BINDIR)
	gcc -std=c99 $^ $(LDFLAGS) -o $@

$(BINDIR)/test_edit_buffer: $(OBJDIR)/test_edit_buffer.o $(OBJDIR)/edit_buffer.o
	mkdir -p $(BINDIR)
	gcc -std=c99 $^ $(LDFLAGS) -o $@

$(BINDIR)/test_vec: $(OBJDIR)/test_vec.o $(OBJDIR)/vec2.o
	mkdir -p $(BINDIR)
	gcc -std=c99 $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	gcc -Wall -Werror -pedantic -std=c99 -g -c $< -o $@

clean:
	/bin/rm -rf $(OBJDIR) $(BINDIR)

.PHONY: clean
