#include "io.h"
#include "globconst.h"
#include "raw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void appendRow(char* s, int len) {  //adiciona nova linha
  E.rows = realloc(E.rows, sizeof(rowOfText) * (E.numRows + 1));

  int at = E.numRows;

  E.rows[at].size = len;
  E.rows[at].text = malloc(len+1);
  memcpy(E.rows[at].text, s, len);
  E.rows[at].text[len] = '\0';
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