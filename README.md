# Ncurses Text Editor (WIP)

**Status: In Progress**

A terminal-based text editor built in C++ using the `ncurses` library, inspired by **Vim**. Designed to be lightweight, modular, and efficient—with tiling windows, multiple buffers, and Vim-like modal editing.

---

## Build

```sh
git clone https://github.com/aarya-bhatia/ncurses-editor.git 
cd ncurses-editor
make
```

## Run

```sh
bin/main [<filename1> [<filename2>...]]
```

## Test

```sh
bin/test
```

## Logs

Program will create and append logs to the file `stderr.log` in the project directory. View logs in another terminal with `tail -f stderr.log`.

---

## Features (In Progress)

- Tiling window management using a binary tree layout
- Tab and buffer management for multiple open files
- Vim-style modal editing: Insert, Normal, and Command modes
- Lazy redraw: only update modified parts of the screen
- Graceful terminal resizing
- Plugin-like file event handlers (e.g., `:Ex` to explore files)
- Chord-style keybindings for navigation and actions
- Modular and extensible architecture

---

## Screenshots

![Screenshot 1](https://github.com/aarya-bhatia/ncurses-editor/blob/main/assets/Screenshot%202025-05-27%20at%2011.20.53%E2%80%AFPM.png)  
![Screenshot 2](https://github.com/aarya-bhatia/ncurses-editor/blob/main/assets/Screenshot%202025-05-27%20at%2011.21.31%E2%80%AFPM.png)

---

## Insert Mode

| Key           | Action                      |
|---------------|-----------------------------|
| `<Esc>`       | Exit insert mode            |

---

## Command Mode

| Command             | Description                          |
|---------------------|--------------------------------------|
| `:q[uit]`            | Exit program |
| `<Esc>`             | Exit command mode                    |
| `<Enter>`           | Execute command                      |
| `:<line>`           | Jump to specified line               |
| `:sp`, `:vs`           | create horizontal/vertical split               |
| `:right`, `:left`, `:top`, `:bottom`           | navigate to window pane |
| `:tabnext` / `:tabprev`   | Switch to next or previous tab      |
| `:edit <filename>` / `:open <filename>`  | Open a file if it exists             |
| `:close`            | Close current file                   |
| `:tabclose`            | Close current tab                   |
| `:save`            | Save current file to disk.                   |
| `:tabnew`            | Open and switch to new tab.                   |
| `:copy`            | Copy current line                   |
| `:Ex`            | Open file explorer in buffer                   |

---

## Normal Mode

| Key       | Action                                     |
|-----------|--------------------------------------------|
| `h`       | Move left                                  |
| `j`       | Move down                                  |
| `k`       | Move up                                    |
| `l`       | Move right                                 |
| `0`       | Jump to start of line                      |
| `$`       | Jump to end of line                        |
| `i`       | Enter insert mode                          |
| `:`       | Enter command mode                         |
| `G`       | Go to end of file                          |
| `gg`       | Go to start of file                        |
| `yy`       | Copy the current line                        |
| `p`       | Paste copied line below                        |
| `x`       | Erase character at cursor                        |
| `o`       | Insert line below cursor                        |
| `dd`       | Delete current line                        |

---

## Credits

- [ncurses](https://github.com/mirror/ncurses) for terminal rendering
- [Catch2](https://github.com/catchorg/Catch2) for unit testing

---

## Roadmap / TODO

### Core

- [x] Auto-scroll when cursor jumps off-screen
- [ ] Efficient line redraw after inserts/deletes
- [ ] Handle empty buffer states
- [ ] Read filenames from command-line arguments
- [ ] Open and edit multiple files
- [ ] Use buffer IDs instead of filenames

### Features

- [ ] Line number display
- [ ] Run external shell commands on current line
- [ ] Visual mode
- [ ] Popup UI for long messages
- [ ] Command history
- [ ] Color support
