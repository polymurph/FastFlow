#include "../hal/oven.h"

#include "gpio.h"
#include "main.h"

void oven_init()
{

}

bool oven_door_is_closed()
{
	return HAL_GPIO_ReadPin(DOOR_CLOSED_GPIO_Port, DOOR_CLOSED_Pin);
}
