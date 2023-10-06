#ifndef _H_GLOBCONST
#define _H_GLOBCONST

#include <termios.h>

struct editorConfig {
  struct termios orig_termios;
  int screenrows;
  int screencols;
};

struct abuf {
  char* buf;
  int len;
};

#define ABUF_INIT {NULL, 0}

extern struct editorConfig E;

#endif
