#include <unistd.h>

void refreshScreen(void) {
  write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7);
}
