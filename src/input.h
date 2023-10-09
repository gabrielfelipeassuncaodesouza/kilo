#ifndef _H_INPUT
#define _H_INPUT

#include "globconst.h"

#define CTRL_KEY(k) ((k) & 0x1f)

int editorReadKey(void);
void editorProcessKeyPress(void);
int getCursorPosition(int* rows, int* cols);
void abAppend(struct abuf* ab, const char* s, int len);
void abFree(struct abuf* ab);
void rowAppendString(rowOfText* row, char* s, int len);
#endif
