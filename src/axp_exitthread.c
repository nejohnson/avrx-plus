/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

void axpExitThread(void)
{
   axpTerminate(axpSelf());
}
