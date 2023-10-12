#include "input.h"
#include "io.h"
#include "globconst.h"
#include "output.h"
#include "raw.h"

#include <stdio.h>

struct editorConfig E;

void initEditor(void) {
  E.cx = 0;
  E.cy = 0;
  E.rx = 0;
  E.numRows = 0;
  E.rowOffset = 0;
  E.colOffset = 0;
  E.rows = NULL;
  E.filename = NULL;
  E.statusMsg[0] = '\0';
  E.statusMsg_time = 0;
  E.dirty = 0;

  if(getWinSize(&E.screenrows, &E.screencols) == -1)
    die("getWinSize");

  E.screenrows-=2;
}

int main(int argc, char** argv) {
  raw();
  initEditor();
  if(argc >= 2) {
    editorOpen(argv[1]);
  }

  setStatusMsg("HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find"); 

  while(1) {
    refreshScreen();
    editorProcessKeyPress();
  }

  return 0;
}
