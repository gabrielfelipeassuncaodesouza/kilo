#include "io.h"
#include "globconst.h"
#include "raw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void updateRow(rowOfText* row) {
  int tabs = 0;
  for(int j = 0; j < row->size; j++)
    if(row->text[j] == '\t') tabs++;

  free(row->render);
  row->render = malloc(row->size + tabs*(TAB_STOP - 1) + 1);

  int index = 0;
  for(int j = 0; j < row->size; j++) {

    if(row->text[j] == '\t') {
      row->render[index++] = ' ';
      while(index % TAB_STOP != 0) row->render[index++] = ' ';
    }
    else row->render[index++] = row->text[j];
  }

  row->render[index] = '\0';
  row->rsize = index;
}

void appendRow(char* s, size_t len) {  //adiciona nova linha
  E.rows = realloc(E.rows, sizeof(rowOfText) * (E.numRows + 1));

  int at = E.numRows;

  E.rows[at].size = len;
  E.rows[at].text = malloc(len+1);
  memcpy(E.rows[at].text, s, len);
  E.rows[at].text[len] = '\0';

  E.rows[at].rsize = 0;
  E.rows[at].render = NULL;
  updateRow(&E.rows[at]);

  E.numRows++;
}

void editorOpen(char* fileName) {
  FILE* f = fopen(fileName, "r");
  if(!f || f == NULL) die("fopen");

  char* line = NULL;
  size_t lineCapacity = 0;
  ssize_t len;

  while((len = getline(&line, &lineCapacity, f)) != -1) {
    while(len > 0 && (line[len - 1] == '\n' || line[len-1] == '\r'))
      len--;

    appendRow(line, len);
  }
  free(line);
  fclose(f);
}