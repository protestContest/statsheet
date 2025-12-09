#pragma once
#include "manifest.h"

void LZSSEncode(u8 *data, ResInfo *info);
void PackLZSS(ResInfo *info, FILE *f);
