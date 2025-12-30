#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char *name;
  u32 id;
  u32 size;
  u8 *data;
  char *packMethod;
  char *path;
} ResInfo;

typedef struct {
  char *path;
  u32 count;
  u32 capacity;
  ResInfo *items;
} Manifest;

#define ResHeader(size, type, arg)  (((size) << 8) | ((type) << 4) | (arg))
#define WriteHeader(buf, header)    ((*((u32*)(buf)) = (header)),(void)0)
#define ResLength(data)             (*((u32*)(data)) >> 8)

Manifest *ParseManifest(char *path);
