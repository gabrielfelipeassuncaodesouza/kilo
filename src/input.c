#include "globconst.h"
#include "input.h"
#include "io.h"
#include "output.h"
#include "raw.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void rowInsertChar(rowOfText* row, int at, int c) { //user function
  if(at < 0 || at > row->size) at = row->size;

  row->text = realloc(row->text, row->size + 2);
  memmove(&row->text[at+1], &row->text[at], row->size - at + 1)                 ;

  row->size++;
  row->text[at] = c;
  
  updateRow(row);
  E.dirty++;
} 

void rowDelChar(rowOfText* row, int at) {
  if(at < 0 || at >= row->size) return;

  memmove(&row->text[at], &row->text[at + 1], row->size - at);
  row->size--;
  updateRow(row);
  E.dirty++;
}

void moveCursor(int key) {
  rowOfText* row = (E.cx >= E.numRows) ? NULL : &E.rows[E.cx];

  switch(key) {
    case ARROW_LEFT:
      if(E.cy != 0) E.cy--;
      else if(E.cx > 0) {
        E.cx--;
        E.cy = E.rows[E.cx].size;
      }
      break;
    case ARROW_RIGHT:
      if(row && E.cy < row->size) E.cy++;
      else if(row && E.cy == row->size) {
        E.cx++;
        E.cy = 0;
      }
      break;
    case ARROW_DOWN:
      if(E.cx < E.numRows) E.cx++;
      break;
    case ARROW_UP:
      if(E.cx != 0) E.cx--;
      break;
  }

  row = (E.cx >= E.numRows) ? NULL : &E.rows[E.cx];
  int rowlen = row ? row->size : 0;
  if(E.cy > rowlen) E.cy = rowlen;
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

void insertChar(int c) {
  if(E.cx == E.numRows)
    appendRow("", 0); 

  rowInsertChar(&E.rows[E.cx], E.cy, c);
  E.cy++;
}

void delChar(void) {
  if(E.cx == E.numRows) return;

  rowOfText* row = &E.rows[E.cx];
  if(E.cy > 0) {
    rowDelChar(row, E.cy - 1);
    E.cy--;
  }
}

void editorProcessKeyPress(void) {
  static int quit_times = 3;

  int c = editorReadKey();

  switch(c) {
    case '\r': break;

    case CTRL_KEY('q'):
      if(E.dirty && quit_times) {
        setStatusMsg("WARNING!!! File has unsaved changes. Press Ctrl-Q %d more times to quit.", quit_times--);
        return;
      }

      write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7);
      exit(0);
      break;

    case CTRL_KEY('s'):
      editorSave();
      break;

    case HOME:
      E.cy = 0;
      break;
    case END:
      if(E.cx < E.numRows)
        E.cy = E.rows[E.cx].size;
      break;

    case BACKSPACE:
    case CTRL_KEY('h'):
    case DEL:
      if(c == DEL) moveCursor(ARROW_RIGHT);
      delChar();
      break;

    case PG_UP:
    case PG_DOWN:
      {
        if(c == PG_UP) E.cx = E.rowOffset;
        else if(c == PG_DOWN) {
          E.cx = E.rowOffset + E.screenrows - 1;
          if(E.cx > E.numRows) E.cx = E.numRows;
        }

        int times = E.screenrows;
        while(times--)
          moveCursor(c == PG_UP ? ARROW_UP : ARROW_DOWN);
      }
      break;

    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_RIGHT:
    case ARROW_LEFT:
      moveCursor(c);
      break;

    case CTRL_KEY('l'):
    case '\x1b':
      break;

    default:
      insertChar(c);
      break;
  }

  quit_times = 3;
}
