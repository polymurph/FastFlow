#include "fastFlow.h"
#include "hardware.h"

#include <stdint.h>
#include "usart.h"

#include "encoder.h"

void fastFlow_run()
{
	uint8_t helloWorld_msg[] = "Hello, World!\n\r";
	uint8_t turnedRight_msg[] = "Turned Right!\n\r";
	uint8_t turnedLeft_msg[] = "Turned Left!\n\r";
	uint8_t buttonPressed_msg[] = "Button pressed!\n\r";

	init_hardware();

	HAL_UART_Transmit(&huart2, helloWorld_msg, sizeof(helloWorld_msg) - 1, 1000);


	while(1){

		switch(encoder_read()){
			case MOVED_CLOCKWISE:
				HAL_UART_Transmit(&huart2, turnedRight_msg, sizeof(turnedRight_msg) - 1, 1000);
				break;

			case MOVED_COUNTERCLOCKWISE:
				HAL_UART_Transmit(&huart2, turnedLeft_msg, sizeof(turnedLeft_msg) - 1, 1000);
				break;

			default:
				break;
		}

		if(encoder_readPushButton()){
			HAL_UART_Transmit(&huart2, buttonPressed_msg, sizeof(buttonPressed_msg) - 1, 1000);
		}

		HAL_Delay(100);

	}
}
