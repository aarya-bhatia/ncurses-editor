OBJDIR=.obj
BINDIR=bin
SRCDIR=src
CFLAGS=-c -Wall -Werror -pedantic -g -D_GNU_SOURCE -MMD -MP
LDFLAGS=-lncurses -lm

INCLUDE_DIRS=$(shell find src -type d)
INCLUDES := $(patsubst %, -I%, $(INCLUDE_DIRS))
CFLAGS += $(INCLUDES)

SRC_FILES=$(shell find src -type f -name "*.cpp" -o -name "*.c")
SRC_OBJS=$(SRC_FILES:%=$(OBJDIR)/%.o)

MAIN_FILE=main.cpp
MAIN_OBJ=$(OBJDIR)/$(MAIN_FILE).o

TEST_SRC_FILES=$(shell find tests -type f -name "*.cpp")
TEST_OBJS=$(TEST_SRC_FILES:%=$(OBJDIR)/%.o)

all: main

main: $(BINDIR)/main
test: $(BINDIR)/test

$(BINDIR)/main: $(MAIN_OBJ) $(SRC_OBJS)
	mkdir -p $(dir $@)
	g++ $^ $(LDFLAGS) -o $@
	
$(BINDIR)/test: $(TEST_OBJS) $(SRC_OBJS)
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

-include $(SRC_OBJS:.o=.d) $(TEST_OBJS:.o=.d) $(MAIN_OBJ:.o=.d)
