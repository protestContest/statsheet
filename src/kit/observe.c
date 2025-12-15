#include "kit/observe.h"
#include "kit/debug.h"
#include "kit/math.h"
#include "kit/str.h"
#include "kit/vec.h"
#include "kit/hashmap.h"

typedef struct {
  void *data;
  ObserveFn fn;
} Observer;

static HashMap subjectMap = EmptyHashMap;
static Observer **subjects = 0;

void Observe(void *subject, void *data, ObserveFn fn)
{
  u32 index;
  u32 key = Hash(&subject, sizeof(void*));

  if (!HashMapFetch(&subjectMap, key, &index)) {
    VecPush(subjects, 0);
    index = VecCount(subjects) - 1;
    HashMapSet(&subjectMap, key, index);
  }

  Observer obs = {data, fn};
  VecPush(subjects[index], obs);
  Assert(VecCount(subjects[index]) > 0);
}

void Notify(void *subject, u32 event)
{
  u32 index;
  u32 key = Hash(&subject, sizeof(void*));

  if (!HashMapFetch(&subjectMap, key, &index)) return;

  Observer *observers = subjects[index];
  for (u32 i = 0; i < VecCount(observers); i++) {
    observers[i].fn(observers[i].data, subject, event);
  }
}
