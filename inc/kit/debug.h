#pragma once

#define Assert(cond)  (void)((cond) || (AssertFail(__FILE__, __LINE__),0))

bool EnableDebug(void);
void Log(char *msg);
void Error(char *msg);

void AssertFail(char *file, u32 line);
