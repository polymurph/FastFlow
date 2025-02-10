#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include "../hal/display.h"

void hw_init();

display_t* hw_getDisplayobjectByPtr();

uint32_t hw_getTick();

void hw_toggleLED();

#endif // _HARDWARE_H_
