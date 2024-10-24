#include "hardware.h"

#include <stdbool.h>

#include "i2c.h"
#include "usart.h"
#include "tim.h"

#include "pcf8575.h"

#include "buzzer.h"
#include "encoder.h"
#include "ui.h"

static pcf8575_t ioexpander;

static display_t display;

const uint32_t c_displayRefreshRateNTick = 1;
const uint32_t c_encoderRefreshRateNTick = 200;
const uint32_t c_LEDRefreshRateNTick = 250;



// interface functions pcf8575
void _i2cWrite(uint8_t address, uint8_t* data,uint8_t len);
void _i2cRead(uint8_t address, uint8_t* data,uint8_t len);

// interface functions display
void _disp_enable(bool state);
void _disp_delay();
void _disp_writePort(uint8_t data);
void _disp_readWrite(bool state);
void _disp_regSelect(bool state);


void hw_init()
{
	uint8_t rowIndex = 0;
	uint8_t rowIndex_old = 0;

	uint32_t tickNow = HAL_GetTick();
	uint32_t displayLastTick = tickNow;
	uint32_t encoderLastTick = tickNow;
	uint32_t LEDLastTick = tickNow;

	//char clear[] = "                                                                ";

	uint8_t turnedRight_msg[] = "Turned Right!\n\r";
	uint8_t turnedLeft_msg[] = "Turned Left!\n\r";

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

	// seccond level
	pcf8575_init(&ioexpander, 0x20, _i2cRead, _i2cWrite, 0x00, 0x00);

	encoder_init();

	buzzer_init();


	// third level

	display_init(
			&display,
			_disp_delay,
			_disp_writePort,
			_disp_readWrite,
			_disp_enable,
			_disp_regSelect);

	//ui_init(&display);

	/*
	while(1){
		tickNow = HAL_GetTick();


		if(tickNow - encoderLastTick > c_encoderRefreshRateNTick){
			encoderLastTick = tickNow;
			ui_updateRoutine();
		}

		if(tickNow - LEDLastTick > c_LEDRefreshRateNTick){
			LEDLastTick = tickNow;
			pcf8575_togglePin(&ioexpander, PCF8575_IOPORT_0, 7);
		}


		if(tickNow - displayLastTick > c_displayRefreshRateNTick){
			displayLastTick = tickNow;
			display_updateRoutine(&display);
		}
	}
	*/
}


display_t* hw_getDisplayobjectByPtr()
{
	return &display;
}

uint32_t hw_getTick()
{
	return HAL_GetTick();
}

void hw_toggleLED()
{
	pcf8575_togglePin(&ioexpander, PCF8575_IOPORT_0, 7);
}

// interface functions pcf8575

void _i2cWrite(uint8_t address, uint8_t* data,uint8_t len)
{
	HAL_I2C_Master_Transmit(&hi2c1, address, data, len, 10000);
}

void _i2cRead(uint8_t address, uint8_t* data, uint8_t len)
{
	HAL_I2C_Master_Receive(&hi2c1, address, data, len, 10000);
}

// interface functions display

void _disp_enable(bool state)
{
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 2, state);
}

void _disp_delay()
{
	uint32_t i = 500;
	for(;i>0;i--);
	//HAL_Delay(1);
}

void _disp_writePort(uint8_t data)
{
	pcf8575_writePort(&ioexpander, PCF8575_IOPORT_1, data);
}

void _disp_readWrite(bool state)
{
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 1, state);
}

void _disp_regSelect(bool state)
{
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 0, state);
}
