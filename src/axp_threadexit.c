/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

void axpThreadExit(void)
{
   axpTerminate(axpSelf());
}
