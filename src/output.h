#ifndef _H_OUTPUT
#define _H_OUTPUT

#include "globconst.h"

void refreshScreen(void);
void drawRows(struct abuf* ab);
void drawStatusBar(struct abuf* ab);
void setStatusMsg(const char* fmt, ...);
char* editorPrompt(char* prompt, void (*callback)(char*, int));
int editorRowRxToCy(rowOfText* row, int rx);

#endif
