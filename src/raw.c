#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void cbreak() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void raw(void) {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(cbreak);
  struct termios raw = orig_termios;

  raw.c_iflag &= ~(IXON | ICRNL);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
