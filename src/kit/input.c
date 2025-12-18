#include "kit/input.h"
#include "kit/hardware.h"
#include "kit/interrupt.h"
#include "kit/debug.h"
#include "kit/time.h"

static void Wake(void)
{
  DisableInterrupts(INT_KEYPAD);
  REG_KEYCNT = 0;
}

void SleepUntil(u16 keys)
{
  REG_KEYCNT = keys | 0xC000;
  OnInterrupt(INT_KEYPAD, Wake);
  Stop();
}

void WaitForInput(void)
{
  while ((~REG_KEYINPUT) & BTN_ANY) {}

  REG_KEYCNT = BTN_ANY | 0x4000;
  OnInterrupt(INT_KEYPAD, Wake);
  IntrWait(true, INT_KEYPAD);
}

static u16 keys = 0;
static u16 lastKeys = 0;
static u32 autokeyStart = 0;
static u32 autoKeyTime = 10;

u16 GetInput(void)
{
  lastKeys = keys;
  keys = ~REG_KEYINPUT;

  if (KeyPressed(BTN_ANY)) {
    autokeyStart = TickCount();
    autoKeyTime = 20;
  } else if (TickCount() - autokeyStart > autoKeyTime) {
    autokeyStart = TickCount();
    autoKeyTime = 5;
  }

  return keys;
}

bool KeyPressed(u16 btn)
{
  return (lastKeys ^ keys) & keys & btn;
}

bool KeyReleased(u16 btn)
{
  return (lastKeys ^ keys) & lastKeys & btn;
}

bool AutoKey(u16 btn)
{
  if (!(btn & keys)) return false;
  return TickCount() - autokeyStart == autoKeyTime;
}
