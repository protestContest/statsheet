#pragma once
#include "manifest.h"
#include <stdio.h>
#include <stdlib.h>

void LZSSEncode(u8 *data, ResInfo *info);
void PackLZSS(ResInfo *info, FILE *f);
