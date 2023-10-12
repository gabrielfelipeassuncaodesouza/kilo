#ifndef _H_IO
#define _H_IO

#include "globconst.h"

void editorOpen(char *fileName);
void updateRow(rowOfText* row);
void insertRow(int at, char* s, size_t len);
void editorSave(void);
void delRow(int at);
void editorFind(void);

#endif
