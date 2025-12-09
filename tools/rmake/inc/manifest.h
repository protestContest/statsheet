#pragma once

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

Manifest *ParseManifest(char *path);
