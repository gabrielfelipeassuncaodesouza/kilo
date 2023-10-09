#include "io.h"
#include "globconst.h"
#include "output.h"
#include "raw.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
  E.dirty++;
}

void editorOpen(char* fileName) {
  free(E.filename);
  E.filename = strdup(fileName); 

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
  E.dirty = 0;
}

char* rowsToString(int* len) {
  int totalLen = 0;
  for(int j = 0; j < E.numRows; j++)
    totalLen += E.rows[j].size + 1;

  *len = totalLen;
  char* buf = malloc(totalLen);
  char* p = buf;

  for(int j = 0; j < E.numRows; j++) {
    memcpy(p, E.rows[j].text, E.rows[j].size);
    p += E.rows[j].size;
    *p = '\n';
    p++;
  }

  return buf;
}

void editorSave(void) {
  if(E.filename == NULL) return;

  int len;
  char* buf = rowsToString(&len);

  int fd = open(E.filename, O_RDWR | O_CREAT, 0644);

  if(fd != -1) {
    if(ftruncate(fd, len) != -1) {
      if(write(fd, buf, len) == len) {
        close(fd);
        free(buf);
        E.dirty = 0;
        setStatusMsg("%d bytes written to disk", len);
        return;
      }
    }
    close(fd);
  }

  free(buf);
  setStatusMsg("Can't save! I/O error: %s", strerror(errno));
}
