#include "hardware.h"

#include "buzzer.h"
#include "encoder.h"

void init_hardware()
{
	encoder_init();
	buzzer_init();
}
