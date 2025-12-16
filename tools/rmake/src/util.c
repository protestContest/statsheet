#include "util.h"
#include <stdio.h>
#include <stdlib.h>

u32 FileSize(char *path)
{
  FILE *file = fopen(path, "r+b");
  if (!file) {
    fprintf(stderr, "Could not open resource \"%s\"\n", path);
    exit(2);
  }

  fseek(file, 0, SEEK_END);
  u32 size = ftell(file);
  fclose(file);
  return size;
}


