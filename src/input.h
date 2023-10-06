#ifndef _H_INPUT
#define _H_INPUT

#define CTRL_KEY(k) ((k) & 0x1f)

char editorReadKey(void);
void editorProcessKeyPress(void);
int getCursorPosition(int* rows, int* cols);

#endif
