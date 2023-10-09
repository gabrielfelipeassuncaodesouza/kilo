#ifndef _H_IO
#define _H_IO

#include "globconst.h"

void editorOpen(char *fileName);
void updateRow(rowOfText* row);
void appendRow(int at, char* s, size_t len);
void editorSave(void);
void delRow(int at);

#endif
