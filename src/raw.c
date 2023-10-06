#include "input.h"
#include "globconst.h"
#include "output.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

int getWinSize(int* rows, int* cols) {
  struct winsize ws;

  if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    if(write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
    return getCursorPosition(rows, cols);
  }
  else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

void die(const char* msg) {
  refreshScreen();

  perror(msg);
  exit(1);
}

void cbreak() {
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
    die("tcsetattr");
}

void raw(void) {
  if(tcgetattr(STDIN_FILENO, &E.orig_termios) == -1)
    die("tcgetattr");

  atexit(cbreak);
  struct termios raw = E.orig_termios;

  raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

  //Timeout for read
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tcsetattr");
}
