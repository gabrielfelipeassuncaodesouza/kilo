#include "input.h"
#include "output.h"
#include "raw.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

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
  }
}

