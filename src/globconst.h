#ifndef _H_GLOBCONST
#define _H_GLOBCONST

/** PORTABLE DEFINES **/
#define _DEFAULT_SOURCE
#define _BSD_SOURCE
//#define _WIN_SOURCE

#include <termios.h>

enum editorKey {
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  PG_UP,
  PG_DOWN,
  HOME,
  END,
  DEL
};

typedef struct rowOfText {
  int size;
  char* text;
} rowOfText;

struct editorConfig {
  struct termios orig_termios;
  int screenrows;
  int screencols;
  int cx;
  int cy;
  int numRows;
  rowOfText row;
};

struct abuf {
  char* buf;
  int len;
};

#define ABUF_INIT {NULL, 0}

extern struct editorConfig E;

#endif
