#include "raw.h"
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

int main() {
  raw();

  while(1) {
    char c;
    if(read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
      die("read");

    if(iscntrl(c)) {
      printf("%d\r\n", c);
    }
    else {
      printf("%d ('%c')\r\n", c, c);
    }
    if(c == 'q') break;
  }

  return 0;
}
