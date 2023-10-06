#include "input.h"
#include "io.h"
#include "globconst.h"
#include "output.h"
#include "raw.h"

struct editorConfig E;

void initEditor(void) {
  E.cx = 0;
  E.cy = 0;
  E.numRows = 0;

  if(getWinSize(&E.screenrows, &E.screencols) == -1)
    die("getWinSize");
}

int main(int argc, char** argv) {
  raw();
  initEditor();
  if(argc >= 2) {
    editorOpen(argv[1]);
  }

  while(1) {
    refreshScreen();
    editorProcessKeyPress();
  }

  return 0;
}
