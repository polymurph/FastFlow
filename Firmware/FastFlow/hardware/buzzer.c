#include "buzzer.h"

#include "main.h"
#include "tim.h"

void buzzer_init()
{
	//HAL_TIM_Stop(&htim4);
}

void buzzer_turnOn()
{
	HAL_TIM_Start(&htim4);
}

void buzzer_turnOff()
{
	HAL_TIM_Stop(&htim4);
}
