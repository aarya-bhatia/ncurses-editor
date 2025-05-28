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

## Current Features

- Tiling window management with binary tree
- Tab management
- file buffer management
- Handles resize gracefully
- Fast: Lazily redraw parts of window
- Vim-like - Insert/Command/Normal mode support
- chord-like Keybindings
- support custom file event handlers to use as plugins - For example, the `:Ex` command will list files in a directory and open them on selection.

## Screenshots

![screenshot 1](https://github.com/aarya-bhatia/ncurses-editor/blob/main/assets/Screenshot%202025-05-27%20at%2011.20.53%E2%80%AFPM.png)

![screenshot 2](https://github.com/aarya-bhatia/ncurses-editor/blob/main/assets/Screenshot%202025-05-27%20at%2011.21.31%E2%80%AFPM.png)

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
- TODO core: add logic to update specific lines after inserts/deletes in editor class.
- TODO core: read filenames from args on launch
- TODO core: open and edit multiple files
- TODO feat: show line numbers
- DONE core: auto-scroll when jumping cursor goes off-screen
- TODO core: handle empty buffer
- TODO feat: command mode - run external commands on current line
- TODO feat: visual mode
- TODO idea: impl popup to show longer info messages to user
- TODO feat: keep user command history
- TODO improv: add colors
- TODO core: use buffer ids instead of filenames
