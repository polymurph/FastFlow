
#ifndef _TEMPSENSOR_H_
#define _TEMPSENSOR_H_

#include <stdint.h>

void tempSensor_inti();

float tempSensor_ReadTemperatureCelsius();

uint16_t tempSensor_ReadADC();

#endif // TEMPSENSOR_H_
