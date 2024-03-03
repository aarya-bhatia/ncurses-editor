OBJDIR=.obj
BINDIR=bin
SRCDIR=.
SRC=$(shell ls *.c | grep -v '^test')
OBJS=$(SRC:%.c=$(OBJDIR)/%.o)

all: $(BINDIR)/editor $(BINDIR)/test_edit_buffer

$(BINDIR)/editor: $(OBJDIR)/editor.o $(OBJDIR)/edit_buffer.o
	mkdir -p $(BINDIR)
	gcc -std=c99 $^ -lncurses -o $@

$(BINDIR)/test_edit_buffer: $(OBJDIR)/test_edit_buffer.o $(OBJDIR)/edit_buffer.o
	mkdir -p $(BINDIR)
	gcc -std=c99 $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	gcc -Wall -Werror -pedantic -std=c99 -g -c $< -o $@

clean:
	/bin/rm -rf $(OBJDIR) $(BINDIR)

.PHONY: clean
