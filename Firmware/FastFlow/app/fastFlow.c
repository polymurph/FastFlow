#include "fastFlow.h"
#include "hardware.h"

#include <stdint.h>
#include <stdbool.h>

#include "usart.h"

#include "encoder.h"
#include "buzzer.h"
#include "tim.h"
#include "i2c.h"
#include "gpio.h"

#include "display.h"
#include "pcf8575.h"

#include "stm32f4xx_hal_i2c.h"


void _i2cWrite(uint8_t address, uint8_t* data,uint8_t len);
void _i2cRead(uint8_t address, uint8_t* data,uint8_t len);


void fastFlow_run()
{
	uint8_t helloWorld_msg[] = "Hello, World!\n\r";
	uint8_t turnedRight_msg[] = "Turned Right!\n\r";
	uint8_t turnedLeft_msg[] = "Turned Left!\n\r";
	uint8_t turnedRightFast_msg[] = "Turned Right Fast!\n\r";
	uint8_t turnedLeftFast_msg[] = "Turned Left Fast!\n\r";
	uint8_t buttonPressed_msg[] = "Button pressed!\n\r";

	uint8_t data_0[2] = {0xFF,0xFF};
	uint8_t data_1[2] = {0x00,0x00};

	uint8_t dump[2] = {0};

	pcf8575_t ioexpander;

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	pcf8575_init(&ioexpander, 0x20, _i2cRead, _i2cWrite, 0xFF, 0xFF);

	init_hardware();

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

	HAL_UART_Transmit(&huart2, helloWorld_msg, sizeof(helloWorld_msg) - 1, 1000);


	GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD; // Open-Drain Alternate Function
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1; // AF4 for I2C1
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//HAL_I2C_Master_Receive(&hi2c1, 0x02 << 1, dump, 2, 0);


	// Reset I2C1 Peripheral
	//__HAL_RCC_I2C1_FORCE_RESET();
	//__HAL_RCC_I2C1_RELEASE_RESET();

	// Re-initialize I2C1
	HAL_I2C_DeInit(&hi2c1);
	HAL_I2C_MspInit(&hi2c1);
	HAL_I2C_Init(&hi2c1);


	while(1){
/*
		switch(encoder_read()){
			case MOVED_CLOCKWISE:
				HAL_UART_Transmit(&huart2, turnedRight_msg, sizeof(turnedRight_msg) - 1, 1000);
				break;

			case MOVED_COUNTERCLOCKWISE:
				HAL_UART_Transmit(&huart2, turnedLeft_msg, sizeof(turnedLeft_msg) - 1, 1000);
				break;

			case MOVED_CLOCKWISE_FAST:
				HAL_UART_Transmit(&huart2, turnedRightFast_msg, sizeof(turnedRightFast_msg) - 1, 1000);
				break;

			case MOVED_COUNTERCLOCKWISE_FAST:
				HAL_UART_Transmit(&huart2, turnedLeftFast_msg, sizeof(turnedLeftFast_msg) - 1, 1000);
				break;

			default:
				break;
		}

		if(encoder_readPushButton()){
			HAL_UART_Transmit(&huart2, buttonPressed_msg, sizeof(buttonPressed_msg) - 1, 1000);
			pcf8575_togglePin(&ioexpander, PCF8575_IOPORT_1, 0);
			//pcf8575_togglePin(&ioexpander, PCF8575_IOPORT_0, 0);
		}

		//display_updateRoutine();
		HAL_Delay(100);
		*/
		//pcf8575_togglePin(&ioexpander, PCF8575_IOPORT_1, 0);

		pcf8575_writePin(&ioexpander, PCF8575_IOPORT_1, 0,true);
		HAL_Delay(1000);
		pcf8575_writePin(&ioexpander, PCF8575_IOPORT_1, 0,false);
		HAL_Delay(1000);


		/*
		if(HAL_I2C_Master_Transmit(&hi2c1, 0x20 << 1, data_0, 2, 1000)!=HAL_OK){
			while(1);
		}
		HAL_Delay(1000);
		if(HAL_I2C_Master_Transmit(&hi2c1, 0x20 << 1, data_1, 2, 1000)!=HAL_OK){
			while(1);
		}
		HAL_Delay(1000);
		*/
	}
}

void _i2cWrite(uint8_t address, uint8_t* data,uint8_t len)
{
	HAL_I2C_Master_Transmit(&hi2c1, address, data, len, 10000);
}

void _i2cRead(uint8_t address, uint8_t* data, uint8_t len)
{
	HAL_I2C_Master_Receive(&hi2c1, address, data, len, 10000);
}
