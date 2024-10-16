#include "hardware.h"

#include "buzzer.h"
#include "encoder.h"
#include "display.h"

void init_hardware()
{
	encoder_init();
	buzzer_init();
	display_init();
}
