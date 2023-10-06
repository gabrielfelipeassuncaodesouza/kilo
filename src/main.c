#include "input.h"
#include "globconst.h"
#include "output.h"
#include "raw.h"

struct editorConfig E;

void initEditor(void) {
  E.cx = 0;
  E.cy = 0;

  if(getWinSize(&E.screenrows, &E.screencols) == -1)
    die("getWinSize");
}

int main() {
  raw();
  initEditor();

  while(1) {
    refreshScreen();
    editorProcessKeyPress();
  }

  return 0;
}
