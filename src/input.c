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

void moveCursor(char key) {
  switch(key) {
    case 'a':
      E.cy--;
      break;
    case 'd':
      E.cy++;
      break;
    case 's':
      E.cx++;
      break;
    case 'w':
      E.cx--;
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

char editorReadKey(void) {
  int nread;
  char c;
  while((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if(nread == -1 && errno != EAGAIN)
      die("read");
  }

  return c;
}

void editorProcessKeyPress(void) {
  char c = editorReadKey();

  switch(c) {
    case CTRL_KEY('q'):
      refreshScreen();
      exit(0);
      break;

    case 'a':
    case 's':
    case 'd':
    case 'w':
      moveCursor(c);
      break;
  }
}
