#include "hardware.h"

#include <stdbool.h>

#include "i2c.h"
#include "usart.h"

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
	uint8_t rowIndex = 0;
	uint8_t rowIndex_old = 0;

	char clear[] = "                                                                ";
	char text[] = "         haha";

	uint8_t turnedRight_msg[] = "Turned Right!\n\r";
	uint8_t turnedLeft_msg[] = "Turned Left!\n\r";

	// seccond level
	pcf8575_init(&ioexpander, 0x20, _i2cRead, _i2cWrite, 0x00, 0x00);

	encoder_init();
	buzzer_init();

	// regSel 0
	// readwrie 1
	// en 2




	// third level

	display_init(
			_disp_delay,
			_disp_writePort,
			_disp_readWrite,
			_disp_enable,
			_disp_regSelect);


	display_print(clear, sizeof(clear), 0, 0);
	display_request(SET_CURSOR_POSITION, 0, 0);
	while(display_updateRoutine());



#if 0
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

	//display_print(clear, sizeof(clear), 0, 0);
	//display_print(clear, sizeof(clear), 1, 0);
	//display_print(clear, sizeof(clear), 2, 0);
	//display_print(clear, sizeof(clear), 3, 0);
	//display_request(SET_CURSOR_POSITION, 0, 0);

	//display_request(UPDATE_DISPLAY,0,0);
	//display_request(UPDATE_DISPLAY, 0, 0);
	//while(display_updateRoutine());


#endif

	while(1){
		switch(encoder_read()){
			case MOVED_COUNTERCLOCKWISE:
				HAL_UART_Transmit(&huart2, turnedLeft_msg, sizeof(turnedLeft_msg) - 1, 1000);
				rowIndex--;
				break;

			case MOVED_CLOCKWISE:
				HAL_UART_Transmit(&huart2, turnedRight_msg, sizeof(turnedRight_msg) - 1, 1000);
				rowIndex++;
				break;
			default:
				break;
		}
		rowIndex &= 0x03;
		if(rowIndex != rowIndex_old){
			display_request(SET_CURSOR_POSITION, rowIndex, 0);
			while(display_updateRoutine());
			rowIndex_old = rowIndex;
		}
		pcf8575_togglePin(&ioexpander, PCF8575_IOPORT_0, 7);
		HAL_Delay(100);
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
