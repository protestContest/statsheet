#pragma once

typedef struct {
  u32 capacity;
  u32 count;
  u32 itemSize;
  void *items;
} Vec;

#define EmptyVec(type) {0, 0, sizeof(type), 0}

void InitVec(Vec *vec, u32 itemSize, u32 capacity);
void ResizeVec(Vec *vec, u32 capacity);
void VecPush(Vec *vec, void *item);
void VecGet(Vec *vec, u32 index, void *dst);
void *VecAt(Vec *vec, u32 index);
