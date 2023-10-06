#include "globconst.h"
#include "input.h"
#include "output.h"
#include <stdio.h>
#include <unistd.h>

void refreshScreen(void) {
  struct abuf ab = ABUF_INIT;

  abAppend(&ab, "\x1b[?25l", 6);
  abAppend(&ab, "\x1b[H", 3);
  drawRows(&ab);

  abAppend(&ab, "\x1b[H", 3);
  abAppend(&ab, "\x1b[?25h", 6);

  write(STDOUT_FILENO, ab.buf, ab.len);
  abFree(&ab);
}

void drawRows(struct abuf* ab) {
  for(int x = 0; x < E.screenrows; x++) {

    if(x == E.screenrows / 3) {
      char welcome[80];
      int welcomelen = snprintf(welcome, sizeof(welcome), "Kilo editor -- version 1.0");

      if(welcomelen > E.screencols) welcomelen = E.screencols;
      int padding = (E.screencols - welcomelen) / 2;

      if(padding--) {
        abAppend(ab, "~", 1);
      }

      while(padding--) abAppend(ab, " ", 1);
      abAppend(ab, welcome, welcomelen);
    }
    else {
      abAppend(ab, "~", 1);
    }

    abAppend(ab, "\x1b[K", 3);
    if(x < E.screenrows - 1) {
      abAppend(ab, "\r\n", 2);
    }
  }
}
