#include "keyboard.h"
#include <string.h>

void *start_key_listener(void *args) {
  Keyboard *kb = (Keyboard *)args;

  char buf[MAX_KEY_CHORD_LENGTH];
  int len = 0;

  while (1) {
    int ch = wgetch(kb->win_editor);
    buf[len++] = ch;

    KeyEvent ev;
    memcpy(ev.keys, buf, len);
    ev.len = len;
    kb->key_handler(ev, kb->args);
    len = 0;
  }

  return kb;
}
