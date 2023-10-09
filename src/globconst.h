#ifndef _H_GLOBCONST
#define _H_GLOBCONST

/** PORTABLE DEFINES **/
#include <time.h>

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#define KILO_VERSION "0.0.1"
#define TAB_STOP 8

#include <termios.h>

enum editorKey {
  BACKSPACE = 127,
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
  int rsize;
  char* render;
} rowOfText;

struct editorConfig {
  struct termios orig_termios;
  int screenrows;
  int screencols;
  int cx;
  int cy;
  int rx;
  int numRows;
  rowOfText* rows;
  int rowOffset;
  int colOffset;
  char* filename;
  char statusMsg[80];
  time_t statusMsg_time;
  int dirty;
};

struct abuf {
  char* buf;
  int len;
};

#define ABUF_INIT {NULL, 0}

extern struct editorConfig E;

#endif
