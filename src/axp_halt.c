/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

void axpHalt(void)
{
   _axpBeginCritical();
   while(1);
}
