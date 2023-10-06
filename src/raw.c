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

  raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

  //Timeout for read
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
