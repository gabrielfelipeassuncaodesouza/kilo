#include "input.h"
#include "globconst.h"
#include "output.h"
#include "raw.h"

struct editorConfig E;

void initEditor(void) {
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
