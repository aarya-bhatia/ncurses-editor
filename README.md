Project is in progress...

# Text Editor

In progress.

This is a terminal-based application made in C++ using the
ncurses library. It is inspired by the vim text
editor.

## Build

```
make
```

## Run

```
bin/editor [<filename1> [<filename2>...] ]
```

## Insert mode
- `<esc>`: exit insert mode
- `<backspace>`: behave like usual backspace key to remove char
- `<enter>`: behave like usual enter key to add another line

## Command mode
- `<esc>`: exit mode without running command
- `<enter>`: exit mode and run command
- `:<line>`: go to line
- `:next`: open next file
- `:prev`: open prev file
- `:open <filename>`: open and load file if exists
- `:close`: close current file
- `:closeall`: close all files
- `:ls`: print open files to log file - TODO show files in a popup

## Normal mode
- `h`: move left
- `j`: move down
- `k`: move up
- `l`: move right
- `0`: jump to start of line
- `$`: jump to end of line
- `f<char>`: jump to next occurrence of character
- `i`: change to insert mode
- `:`: change to command mode
- `I`: start insert mode at start of line
- `a`: start insert mode after cursor (append)
- `A`: start insert mode at end of line
- `G`: go to end of file
- `g`: go to start of file

## Tasks
- TODO feat: read filenames from args on launch
- TODO feat: open and edit multiple files
- TODO feat: show line numbers
- DONE feat: auto-scroll when jumping cursor goes off-screen
- TODO feat: handle empty buffer
- TODO feat: command mode - run external commands on current line
- TODO feat: visual mode
- TODO feat: impl popup to show longer info messages to user
- TODO feat: keep user command history
- TODO improv: add colors
