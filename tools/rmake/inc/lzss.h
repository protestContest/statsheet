#pragma once
#include "manifest.h"
#include <stdio.h>
#include <stdlib.h>

u32 CompressLZSS(void *src, u32 length, u8 **dst);
void PackLZSS(ResInfo *info);
