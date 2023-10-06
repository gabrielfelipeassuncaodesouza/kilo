#include "input.h"
#include "output.h"
#include "raw.h"

int main() {
  raw();

  while(1) {
    refreshScreen();
    editorProcessKeyPress();
  }

  return 0;
}
