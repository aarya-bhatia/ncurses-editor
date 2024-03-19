OBJDIR=.obj
BINDIR=bin
SRCDIR=src
CFLAGS=-c -std=c99 -Wall -Werror -pedantic -g -D_GNU_SOURCE
LDFLAGS=-lncurses -lm

COMMON=$(shell ls $(SRCDIR)/*.c | grep -vE "test|main")
COMMON_OBJ=$(COMMON:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

EDITOR=editor.c main.c
EDITOR_OBJS=$(EDITOR:%.c=$(OBJDIR)/%.o) $(COMMON_OBJ)

all: $(BINDIR)/editor $(BINDIR)/test_edit_buffer $(BINDIR)/test_vec

$(BINDIR)/editor: $(EDITOR_OBJS)
	mkdir -p $(BINDIR)
	gcc -std=c99 $^ $(LDFLAGS) -o $@

$(BINDIR)/test_edit_buffer: $(OBJDIR)/test_edit_buffer.o $(COMMON_OBJ)
	mkdir -p $(BINDIR)
	gcc -std=c99 $^ $(LDFLAGS) -o $@

$(BINDIR)/test_vec: $(OBJDIR)/test_vec.o $(OBJDIR)/vec2.o
	mkdir -p $(BINDIR)
	gcc -std=c99 $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	gcc $(CFLAGS) $< -o $@

clean:
	/bin/rm -rf $(OBJDIR) $(BINDIR)

tags:
	ctags -R --languages=C *

.PHONY: clean tags

