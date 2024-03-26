OBJDIR=.obj
BINDIR=bin
SRCDIR=src
CFLAGS=-c -std=c99 -Wall -Werror -pedantic -g -D_GNU_SOURCE
LDFLAGS=-lncurses -lm

COMMON=$(shell find $(SRCDIR) -type f -name "*.c" | grep -vE "test|main")
COMMON_OBJ=$(COMMON:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

EDITOR=editor.c main.c
EDITOR_OBJS=$(EDITOR:%.c=$(OBJDIR)/%.o) $(COMMON_OBJ)

all: $(BINDIR)/editor $(BINDIR)/test_edit_buffer $(BINDIR)/test_location $(BINDIR)/test_vec

$(BINDIR)/editor: $(EDITOR_OBJS)
	mkdir -p $(dir $@)
	gcc -std=c99 $^ $(LDFLAGS) -o $@

$(BINDIR)/test_%: $(OBJDIR)/test_%.o $(COMMON_OBJ)
	mkdir -p $(dir $@)
	gcc -std=c99 $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	gcc $(CFLAGS) $< -o $@

clean:
	/bin/rm -rf $(OBJDIR) $(BINDIR) vgcore* *.log

tags:
	ctags -R --languages=C *

.PHONY: clean tags

