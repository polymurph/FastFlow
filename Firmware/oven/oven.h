#ifndef _OVEN_H_
#define _OVEN_H_

#include <stdint.h>

// https://www.7pcb.com/blog/lead-free-reflow-profile

typedef struct{
    uint16_t preheatStartingTemperature_C;
    uint16_t warmupRate_CpSec;
    uint16_t preheatEndingtemperature_C;
    uint16_t preheatTime_Sec;
    uint16_t rampRate_CpSec;
    uint16_t peakTemperature_C;
    


}oven_reflowProfile;


#endif // _OVEN_H_
