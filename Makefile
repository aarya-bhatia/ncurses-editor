MACHINE=$(shell uname -s)

ifeq ($(MACHINE), Darwin)
CC=clang
CXX=clang++
else
CC=gcc
CXX=g++
endif

CFLAGS=-std=c99 -Wall -pedantic -g -MMD -MP
CXXFLAGS=-std=c++11 -Wall -pedantic -g -MMD -MP

RM=rm -rf

OBJDIR=.obj
BINDIR=bin
SRCDIR=src
LDFLAGS=-lncurses -lm

INCLUDE_DIRS=$(shell find src -type d)
INCLUDES := $(patsubst %, -I%, $(INCLUDE_DIRS))

CFLAGS += $(INCLUDES)
CXXFLAGS += $(INCLUDES)

SRC_FILES=$(shell find src/core -type f -name "*.cpp" -o -name "*.c")
SRC_OBJS=$(SRC_FILES:%=$(OBJDIR)/%.o)

MAIN_FILE=src/main.cpp
MAIN_OBJ=$(OBJDIR)/$(MAIN_FILE).o

TEST_SRC_FILES=$(shell find tests -type f -name "*.cpp")
TEST_OBJS=$(TEST_SRC_FILES:%=$(OBJDIR)/%.o)

all: main test

main: $(BINDIR)/main
test: $(BINDIR)/test

$(BINDIR)/main: $(MAIN_OBJ) $(SRC_OBJS)
	mkdir -p $(dir $@)
	$(CXX) $^ $(LDFLAGS) -o $@
	
$(BINDIR)/test: $(TEST_OBJS) $(SRC_OBJS)
	mkdir -p $(dir $@)
	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJDIR) $(BINDIR) vgcore* *.dYSM

.PHONY: clean main test

-include $(SRC_OBJS:.o=.d) $(TEST_OBJS:.o=.d) $(MAIN_OBJ:.o=.d)
