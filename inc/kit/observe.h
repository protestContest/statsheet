#pragma once

typedef void (*ObserveFn)(void *data, void *subject, u32 event);

void Observe(void *subject, void *data, ObserveFn fn);
void Notify(void *subject, u32 event);
