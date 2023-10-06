#include "globconst.h"
#include "input.h"
#include "output.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void editorScroll(void) {
  if(E.cx < E.rowOffset)
    E.rowOffset = E.cx;

  if(E.cx >= E.rowOffset + E.screenrows)
    E.rowOffset = E.cx - E.screenrows + 1;
}

void refreshScreen(void) {
  editorScroll();

  struct abuf ab = ABUF_INIT;

  abAppend(&ab, "\x1b[?25l", 6);
  abAppend(&ab, "\x1b[H", 3);

  drawRows(&ab);

  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", ( E.cx - E.rowOffset) + 1, E.cy + 1);
  abAppend(&ab, buf, strlen(buf));

  abAppend(&ab, "\x1b[?25h", 6);

  write(STDOUT_FILENO, ab.buf, ab.len);
  abFree(&ab);
}

void drawRows(struct abuf* ab) {
  for(int x = 0; x < E.screenrows; x++) {

    int fileRow = x + E.rowOffset;

    if(fileRow >= E.numRows) {

      if(E.numRows == 0 && x == E.screenrows / 3) {
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
    }
    else { //parte que printa o arquivo de texto
      int len = E.rows[fileRow].size;
      if(len > E.screencols) len = E.screencols;
      abAppend(ab, E.rows[fileRow].text, len);
    } //até aqui

    abAppend(ab, "\x1b[K", 3); //limpa a linha de baixo
    if(x < E.screenrows - 1) {
      abAppend(ab, "\r\n", 2); //quebra de linha
    }
  }
}