#ifndef _HEATER_H_
#define _HEATER_H_

#include <stdint.h>

void heaterInit();

void heaterSetupTemperaturProfile();

void heaterStart();

void heaterStop();

void heaterRoutine();

uint32_t heaterGetTemp();

#endif // _HEATER_H_
