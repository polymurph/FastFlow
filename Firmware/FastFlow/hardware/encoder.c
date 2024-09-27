#include "encoder.h"

#include "main.h"
#include "tim.h"

#include <stdint.h>

const uint32_t _c_debounce_tick_delay = 10;

void encoder_init()
{
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}

bool encoder_readPushButton()
{
	static uint32_t lastTick = 0;
	static bool lastButtonState = false;
	uint32_t currentTick;
	bool currentButtonState = HAL_GPIO_ReadPin(ENCODER_BUTTON_GPIO_Port, ENCODER_BUTTON_Pin);

	if(currentButtonState && currentButtonState != lastButtonState){
		currentTick = HAL_GetTick();

		if((currentTick - lastTick) > _c_debounce_tick_delay){
			lastTick = currentTick;
			lastButtonState = currentButtonState;
			return true;
		}
	}

	lastTick = currentTick;
	lastButtonState = currentButtonState;
	return false;
}

encoder_movement_t encoder_read()
{
	static uint32_t previousEncoderPosition = 0;

	uint32_t encoderPosition = __HAL_TIM_GET_COUNTER(&htim3);

	int32_t encoderDiff = (int32_t)(encoderPosition - previousEncoderPosition);

	if(encoderDiff == 0){
		return NO_MOVEMENT;
	}

	previousEncoderPosition = encoderPosition;

	if(encoderDiff > 0){
		return MOVED_CLOCKWISE;
	}

	return MOVED_COUNTERCLOCKWISE;
}
