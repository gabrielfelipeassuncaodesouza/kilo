#include "globconst.h"
#include "input.h"
#include "output.h"
#include "raw.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void moveCursor(int key) {
  switch(key) {
    case ARROW_LEFT:
      if(E.cy > 0) E.cy--;
      break;
    case ARROW_RIGHT:
      if(E.cy < E.screencols - 1) E.cy++;
      break;
    case ARROW_DOWN:
      if(E.cx < E.screenrows - 1) E.cx++;
      break;
    case ARROW_UP:
      if(E.cx > 0) E.cx--;
      break;
  }
}

void abAppend(struct abuf* ab, const char* s, int len) {
  char* new = realloc(ab->buf, ab->len + len);

  if(new == NULL) return;
  memcpy(&new[ab->len], s, len);

  ab->buf = new;
  ab->len += len;
}

void abFree(struct abuf* ab) {
  free(ab->buf);
}

int getCursorPosition(int* rows, int* cols) {
  char buf[32];
  unsigned int i = 0;

  if(write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;

  while(i < sizeof(buf) - 1) {
    if(read(STDIN_FILENO, &buf[i], 1) != 1) break;
    if(buf[i] == 'R') break;
    i++;
  }

  buf[i] = '\0';
  if(buf[0] != '\x1b' || buf[1] != '[') return -1;
  if(sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;

  return 0;
}

int editorReadKey(void) {
  int nread;
  char c;
  while((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if(nread == -1 && errno != EAGAIN)
      die("read");
  }

  if(c == '\x1b') {
    char seq[3];
    if(read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
    if(read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

    if(seq[0] == '[') {

      if(seq[1] >= '0' && seq[1] <= '9') {
        if(read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';

        if(seq[2] == '~') {
          switch(seq[1]) {
            case '1': return HOME;
            case '3': return DEL;
            case '4': return END;
            case '5': return PG_UP;
            case '6': return PG_DOWN;
            case '7': return HOME;
            case '8': return END;
          }
        }
      }
      else {
        switch(seq[1]) {
          case 'A': return ARROW_UP;
          case 'B': return ARROW_DOWN;
          case 'C': return ARROW_RIGHT;
          case 'D': return ARROW_LEFT;
          case 'F': return END;
          case 'H': return HOME;
        }
      }
    }
    else if(seq[0] == 'O') {
      switch(seq[1]) {
        case 'F': return END;
        case 'H': return HOME;
      }
    }

    return '\x1b';
  }
  else return c;
}

void editorProcessKeyPress(void) {
  int c = editorReadKey();

  switch(c) {
    case CTRL_KEY('q'):
      refreshScreen();
      exit(0);
      break;

    case HOME:
      E.cy = 0;
      break;
    case END:
      E.cy = E.screencols - 1;
      break;

    case PG_UP:
    case PG_DOWN:
      {
        int times = E.screenrows;
        while(times--)
          moveCursor(c == PG_DOWN ? ARROW_DOWN : ARROW_UP);
      }
      break;

    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_RIGHT:
    case ARROW_LEFT:
      moveCursor(c);
      break;
  }
}
