#ifndef _H_GLOBCONST
#define _H_GLOBCONST

#include <termios.h>

enum editorKey {
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  PG_UP,
  PG_DOWN
};

struct editorConfig {
  struct termios orig_termios;
  int screenrows;
  int screencols;
  int cx;
  int cy;
};

struct abuf {
  char* buf;
  int len;
};

#define ABUF_INIT {NULL, 0}

extern struct editorConfig E;

#endif
