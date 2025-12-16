#include "manifest.h"
#include "parse.h"
#include "util.h"
#include <string.h>
#include <libgen.h>

static void ManifestAppend(Manifest *manifest, ResInfo info)
{
  if (!manifest->capacity) {
    manifest->capacity = 4;
    manifest->items = realloc(manifest->items, sizeof(ResInfo)*manifest->capacity);
  } else if (manifest->capacity < manifest->count + 1) {
    manifest->capacity *= 2;
    manifest->items = realloc(manifest->items, sizeof(ResInfo)*manifest->capacity);
  }

  manifest->items[manifest->count++] = info;
}

static char *PathJoin(char *dir, char *base)
{
  u32 dirLen = strlen(dir);
  u32 baseLen = strlen(base);
  u32 len = dirLen + baseLen + 1;
  char *path = malloc(len + 1);
  path[len] = 0;
  strncpy(path, dir, dirLen);
  path[dirLen] = '/';
  strncpy(path + dirLen + 1, base, baseLen);
  return path;
}

static char *ReadManifest(char *path, u32 *size)
{
  FILE *mFile = fopen(path, "r");
  if (!mFile) {
    fprintf(stderr, "Could not open manifest \"%s\"\n", path);
    exit(1);
  }

  fseek(mFile, 0, SEEK_END);
  *size = ftell(mFile);

  char *contents = malloc(*size);

  fseek(mFile, 0, SEEK_SET);
  fread(contents, *size, 1, mFile);
  fclose(mFile);

  return contents;
}

Manifest *ParseManifest(char *path)
{
  Manifest *manifest = malloc(sizeof(Manifest));
  char *pathCopy = strdup(path);
  manifest->path = dirname(pathCopy); // resource names are relative to the manifest path
  manifest->count = 0;
  manifest->capacity = 0;
  manifest->items = 0;

  u32 mSize;
  char *contents = ReadManifest(path, &mSize);

  // parse the file
  Parser p = {contents, contents + mSize, path, 0, 0};
  while (!AtEnd(&p)) {
    ResInfo info = {0};

    SkipWhitespace(&p);
    if (AtEnd(&p)) break;

    info.name = ParsePath(&p);
    info.id = Hash(info.name, strlen(info.name));

    while (!AtEnd(&p) && IsWhitespace(*p.cur) && *p.cur != '\n') p.cur++;

    if (!AtEnd(&p) && *p.cur != '\n') {
      info.packMethod = ParseName(&p);
    }

    info.path = PathJoin(manifest->path, info.name);
    info.size = FileSize(info.path);

    if (info.size == 0) {
      fprintf(stderr, "Skipping empty resource \"%s\"\n", info.path);
      free(info.path);
      free(info.name);
    } else {
      ManifestAppend(manifest, info);
    }
  }

  free(contents);

  return manifest;
}
