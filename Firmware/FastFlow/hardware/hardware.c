#include "hardware.h"

#include <stdbool.h>

#include "i2c.h"

#include "pcf8575.h"

#include "buzzer.h"
#include "encoder.h"
#include "display.h"

pcf8575_t ioexpander;

// interface functions pcf8575
void _i2cWrite(uint8_t address, uint8_t* data,uint8_t len);
void _i2cRead(uint8_t address, uint8_t* data,uint8_t len);

// interface functions display
void _disp_enable(bool state);
void _disp_delay();
void _disp_writePort(uint8_t data);
void _disp_readWrite(bool state);
void _disp_regSelect(bool state);


void init_hardware()
{
	uint8_t i = 0;

	char clear[] = "                ";
	char text[] = "         haha";
	// seccond level
	pcf8575_init(&ioexpander, 0x20, _i2cRead, _i2cWrite, 0x00, 0x00);

	encoder_init();
	buzzer_init();

	// regSel 0
	// readwrie 1
	// en 2


	/*
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 0, false);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 1, false);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 2, true);
	HAL_Delay(1);
	pcf8575_writePort(&ioexpander, PCF8575_IOPORT_1, 0x38);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 2, false);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 0, true);
	HAL_Delay(1);

	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 0, false);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 1, false);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 2, true);
	HAL_Delay(1);
	pcf8575_writePort(&ioexpander, PCF8575_IOPORT_1, 0x03);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 2, false);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 0, true);
	HAL_Delay(1);

	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 0, false);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 1, false);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 2, true);
	HAL_Delay(1);
	pcf8575_writePort(&ioexpander, PCF8575_IOPORT_1, 0x0E);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 2, false);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 0, true);
	HAL_Delay(1);

	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 0, false);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 1, false);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 2, true);
	HAL_Delay(1);
	pcf8575_writePort(&ioexpander, PCF8575_IOPORT_1, 0x01);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 2, false);
	HAL_Delay(1);
	pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 0, true);
	HAL_Delay(1);

*/



	// third level

	display_init(
			_disp_delay,
			_disp_writePort,
			_disp_readWrite,
			_disp_enable,
			_disp_regSelect);


	//display_request(SET_CURSOR_MODE, VISIBLE_BLINK, 0);

	while(display_updateRoutine());

	//display_request(SET_FUNCTION,0,0);
	display_request(SET_CURSOR_MODE, BLINK, 0);
	//display_request(UPDATE_DISPLAY,0,0);
	//display_request(UPDATE_DISPLAY, 0, 0);
	while(display_updateRoutine());

	display_request(SET_CURSOR_POSITION, 3, 8);
		//display_request(UPDATE_DISPLAY,0,0);
		//display_request(UPDATE_DISPLAY, 0, 0);
	while(display_updateRoutine());

	display_print(clear, sizeof(clear), 0, 0);
	display_print(clear, sizeof(clear), 1, 0);
	display_print(clear, sizeof(clear), 2, 0);
	display_print(clear, sizeof(clear), 3, 0);
	display_request(SET_CURSOR_POSITION, 0, 0);
	//display_request(UPDATE_DISPLAY,0,0);
	//display_request(UPDATE_DISPLAY, 0, 0);
	while(display_updateRoutine());


	while(1){
		pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 7,true);
		HAL_Delay(1000);
		pcf8575_writePin(&ioexpander, PCF8575_IOPORT_0, 7,false);
		HAL_Delay(1000);
	}
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
	HAL_Delay(1);
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
