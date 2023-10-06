#include "io.h"
#include "globconst.h"
#include "raw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void editorOpen(char* fileName) {
  FILE* f = fopen(fileName, "r");
  if(!f || f == NULL) die("fopen");

  char* line = NULL;
  size_t lineCapacity = 0;
  ssize_t len;

  len = getline(&line, &lineCapacity, f);
  if(len != -1) {
    while(len > 0 && (line[len - 1] == '\n' || line[len-1] == '\r'))
      len--;

    E.row.size = len;
    E.row.text = malloc(len+1);
    memcpy(E.row.text, line, len);
    E.row.text[len] = '\0';
    E.numRows++;
  }
  free(line);
  fclose(f);
}
