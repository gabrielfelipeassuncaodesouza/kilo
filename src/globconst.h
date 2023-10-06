#ifndef _H_GLOBCONST
#define _H_GLOBCONST

#include <termios.h>

struct editorConfig {
  struct termios orig_termios;
  int screenrows;
  int screencols;
};

extern struct editorConfig E;

#endif
