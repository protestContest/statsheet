#pragma once

#define Assert(cond)  (void)((cond) || (AssertFail(__FILE__, __LINE__),0))

bool EnableDebug(void);
void Log(char *msg);
void LogNum(i32 num);
void LogHex(u32 hex);
void Error(char *msg);

void AssertFail(char *file, u32 line);
