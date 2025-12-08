#include "kit/init.h"
#include "kit/hardware.h"
#include "kit/interrupt.h"
#include "kit/time.h"

void InitKit(void)
{
#ifdef DEBUG
  EnableDebug();
#endif
  OnInterrupt(INT_VBLANK, UpdateTime);
  GraphicsMode(3);
  ObjMapMode(ObjMap1D);
  ShowLayer(DISP_BG2);
  ShowLayer(DISP_OBJ);
}