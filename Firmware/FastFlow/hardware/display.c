#include "display.h"

#include <string.h>
#include "gpio.h"

#include "stm32f4xx_hal.h"

#if 0
//hardware pin defines
#define DISPLAY_SIGNAL_ENABLE	PINB0
#define DISPLAY_SIGNAL_RW		PINB1
#define DISPLAY_SIGNAL_RS		PINB2
#endif

//display functionality pin setting defines
#define DISPLAY_FUNCTION_SET	0x38
#define DISPLAY_ON				0x0C
#define DISPLAY_OFF				0x08


//register pin setting defines
#define DD_RAM_ADDR_SET			0x80
#define CG_RAM_ADDR_SET			0x40


//cursor mode pin setting defines
#define CURSOR_MODE_AUTO_INCREMENT	0x06
#define CURSOR_MODE_INVISIBLE		0x0C
#define CURSOR_MODE_VISIBLE			0x0E
#define CURSOR_MODE_BLINK			0x0D

static const uint8_t _sc_signarrowLeft = 0x7F;

volatile uint8_t _displayCursorRow = 0;
volatile uint8_t _displayCursorIndex= 0;

char _displayOutputArrayMirror[64] = {0xFF};
static char _displayOutputArray[64] = {0xFF};

uint8_t _writeIndex = 0;
uint8_t _workIndex = 0;

uint8_t _requestPipeline[3][32];

displayStateMachine_modes_t _displayStateMachineMode = setCursorToTop;

display_delay _delay;
display_writePort _writePort;
display_readWrite _readWrite;
display_enable _enable;
display_regSelect _regSelect;

void _setCursorPosition(display_t *object, uint8_t row, uint8_t column);
void _requestHandled(display_t *object);
void _placeRequest(display_t *object, uint8_t mode, uint8_t var1, uint8_t var2);
void _writeCommandToDisplay(display_t *object, uint8_t cmd);
void _writeCharToDisplay(display_t *object, char data);
void _writeToDisplay(display_t *object);
void _updateDisplayArray(display_t *object);


void display_init(
		display_t *object,
		display_delay delay,
		display_writePort writePort,
		display_readWrite readWrite,
		display_enable enable,
		display_regSelect regSelect)
{
	/*
	_delay = delay;
	_writePort = writePort;
	_readWrite = readWrite;
	_enable = enable;
	_regSelect = regSelect;

	_displayCursorRow = 0;
	_displayCursorIndex = 0;
*/
	object->delay = delay;
	object->writePort = writePort;
	object->readWrite =readWrite;
	object->enable = enable;
	object->regSelect = regSelect;

	object->cursorRow = 0;
	object->cursorIndex = 0;

	object->writeIndex = 0;
	object->workIndex = 0;

	display_request(object,SET_FUNCTION, 0, 0);
	//display_request(SET_CURSOR_MODE,INVISIBLE,0);
	display_request(object,SET_CURSOR_MODE,BLINK,0);
	display_request(object,SET_CURSOR_MODE,AUTO_INCREMENT,0);
	display_request(object,UPDATE_DISPLAY,0,0);
}

bool display_updateRoutine(display_t *object)
{
	uint8_t _wIndex;
	// check for new job
	if(!(object->workIndex != object->writeIndex)) return false;

	_wIndex = object->workIndex+1;

	switch (object->requestPipeline[0][_wIndex]) {

		case SET_CURSOR_POSITION:
			_setCursorPosition(object, object->requestPipeline[1][_wIndex], object->requestPipeline[2][_wIndex]);
			_requestHandled(object);
			break;

		case UPDATE_DISPLAY:
			_writeToDisplay(object);
			break;

		case SET_CURSOR_MODE:
			switch (object->requestPipeline[1][_wIndex])
			{
				case INVISIBLE:
					_writeCommandToDisplay(object, CURSOR_MODE_INVISIBLE);
					_requestHandled(object);
					break;

				case VISIBLE:
					_writeCommandToDisplay(object, CURSOR_MODE_VISIBLE);
					_requestHandled(object);
					break;

				case BLINK:
					_writeCommandToDisplay(object,CURSOR_MODE_BLINK);
					_requestHandled(object);
					break;

				case VISIBLE_BLINK:
					_writeCommandToDisplay(object,CURSOR_MODE_BLINK | CURSOR_MODE_INVISIBLE);
					_requestHandled(object);
					break;

				case AUTO_INCREMENT:
					_writeCommandToDisplay(object, CURSOR_MODE_AUTO_INCREMENT);
					_requestHandled(object);
					break;

				default:
					_requestHandled(object);
					break;
			}
			break;

		case SET_FUNCTION:
			_writeCommandToDisplay(object, DISPLAY_FUNCTION_SET);
			_requestHandled(object);
			break;

		default:
		break;
	};
	return true;
}

void display_request(
		display_t *object,
		display_cmd_t cmd,
		uint8_t var1,
		uint8_t var2)
{
	if(cmd == UPDATE_DISPLAY) {
		_updateDisplayArray(object);
		_placeRequest(object, SET_CURSOR_POSITION, 0, 0);
		_placeRequest(object, UPDATE_DISPLAY, 0, 0);
		_placeRequest(object, SET_CURSOR_POSITION, 0, 15);
	} else {
		_placeRequest(object, cmd,var1,var2);
	}
}

void display_print(
		display_t *object,
		char* text,
		uint8_t len,
		uint8_t row,
		uint8_t column)
{
	uint8_t index = 0;

	switch (row){
		case 0:
			index = column;
			break;

		case 1:
			index = column + 16;
			break;

		case 2:
			index = column + 32;
			break;

		case 3:
			index = column + 48;
			break;

		default:
			break;
	}

	memcpy(&(object->outputArrayMirror[index]),text,len);

	display_request(object, SET_CURSOR_POSITION,row,column);
	display_request(object, UPDATE_DISPLAY,0,0);
}

void _setCursorPosition(display_t *object, uint8_t row, uint8_t column)
{
	uint8_t position = 0;

	//this sets up the upper nibble of  the display DD-RAM according to row
	switch(row)
	{
		case 0:
			position = 0x00;
			break;
		case 1:
			position = 0x40;
			break;
		case 2:
			position = 0x10;
			break;
		case 3:
			position = 0x50;
			break;
		//when row exceeds the limit of minimum one row or four rows
		default:
			position = 0x00;
			break;
	}

	//this sets up the lower nibble of  the display DD-RAM according to column
	if(column <= 15 || column >= 0){
		position |= column;
	}else{
		//when column is greater than 16 or smaller then one, it is set to one
		position |= 0;
	}
	_writeCommandToDisplay(object, (position | DD_RAM_ADDR_SET));
}

void _requestHandled(display_t *object)
{
	object->workIndex++;
	object->workIndex &= 0x1F;
}

void _placeRequest(display_t *object, uint8_t mode, uint8_t var1, uint8_t var2)
{
	uint8_t _wIndex;
	object->writeIndex++;
	object->writeIndex &= 0x1F;	//overflow reset

	_wIndex = object->writeIndex;

	object->requestPipeline[0][_wIndex] = mode;
	object->requestPipeline[1][_wIndex] = var1;
	object->requestPipeline[2][_wIndex] = var2;
}

void _writeCommandToDisplay(display_t *object, uint8_t cmd)
{
	//while(read_busy_pin() & PIN_BUSY);

	//clear register select pin
	//HAL_GPIO_WritePin(DISPLAY_REGISTER_SELECT_GPIO_Port, DISPLAY_REGISTER_SELECT_Pin, 0);
	object->regSelect(false);

	//set mode write to display
	//HAL_GPIO_WritePin(DISPLAY_READ_WRITE_GPIO_Port, DISPLAY_READ_WRITE_Pin, 0);
	object->readWrite(false);

	//set enable pin
	//HAL_GPIO_WritePin(DISPLAY_ENABLE_GPIO_Port, DISPLAY_ENABLE_Pin, 1);
	object->enable(true);

	//for(i=0 ; i< 100/*42,43,44,45*/;i++){}
	//HAL_Delay(_delay_ms);
	object->delay();

	//GPIOB->ODR &= 0xF403;
	//GPIOB->ODR |= (cmd & 0x07) | ((cmd & 0x08) << 10) | ((cmd & 0xF0) << 12);
	object->writePort(cmd);

	//for(i=0 ; i< 100/*42,43,44,45*/;i++){}
	//HAL_Delay(_delay_ms);
	object->delay();

	//clear enable pin
	//HAL_GPIO_WritePin(DISPLAY_ENABLE_GPIO_Port, DISPLAY_ENABLE_Pin, 0);
	object->enable(false);

	//for(i=0 ; i< 50/*42,43,44,45*/;i++){}
	//HAL_Delay(_delay_ms);
	object->delay();

	//set resgister select pin
	//HAL_GPIO_WritePin(DISPLAY_REGISTER_SELECT_GPIO_Port, DISPLAY_REGISTER_SELECT_Pin, 1);
	object->regSelect(true);

	//for(i=0 ; i< 50/*42,43,44,45*/;i++){}
	//HAL_Delay(_delay_ms);
	object->delay();
}

void _writeCharToDisplay(display_t *object, char data)
{

	//while(read_busy_pin() & PIN_BUSY);
	uint8_t i=0;

	//set enable pin
	//HAL_GPIO_WritePin(DISPLAY_ENABLE_GPIO_Port, DISPLAY_ENABLE_Pin, 1);
	object->enable(true);


	//for(i=0 ; i< 50;i++){}
	//HAL_Delay(_delay_ms);
	object->delay();

	//set mode write to display
	//HAL_GPIO_WritePin(DISPLAY_READ_WRITE_GPIO_Port, DISPLAY_READ_WRITE_Pin, 0);
	object->readWrite(false);

	//asm volatile ("nop");

	//for(i=0 ; i< 100;i++){}
	//HAL_Delay(_delay_ms);
	object->delay();

	//GPIOB->ODR &= 0xF403;
	//GPIOB->ODR |= (data & 0x07) | ((data & 0x08) << 10) | ((data & 0xF0) << 12);
	object->writePort(data);

	//for(i=0 ; i< 100;i++){}
	//HAL_Delay(_delay_ms);
	object->delay();

	//clear enable pin
	//HAL_GPIO_WritePin(DISPLAY_ENABLE_GPIO_Port, DISPLAY_ENABLE_Pin, 0);
	object->enable(false);

	//for(i=0 ; i< 50;i++){}
	//HAL_Delay(1);
	object->delay();
}

void _writeToDisplay(display_t *object)
{
	switch(object->state){
		case setCursorToTop:
			//cursor set to the first position in row 0 (first position of the first display array)
			_writeCommandToDisplay(object, 0x00|DD_RAM_ADDR_SET);

			//switch mode
			object->state = firstLines;

			//reset display_cursor_index
			object->cursorIndex = 0;
			break;

		case firstLines:
			//write first display array
			_writeCharToDisplay(object, object->outputArray[object->cursorIndex]);
			if (object->cursorIndex == 32){
				 //switch mode when cursor has reached the end of the first display array
				object->state = setCursorToMiddle;
			}else{
				object->cursorIndex++;
			}
			break;

		case setCursorToMiddle:
			//cursor set to the first position in row 1 (first position of the second display array)
			_writeCommandToDisplay(object, 0x40|DD_RAM_ADDR_SET);

			//switch mode
			object->state = secondLines;
			break;

		case secondLines:
			//write second display array
			_writeCharToDisplay(object, object->outputArray[object->cursorIndex]);
			if (object->cursorIndex == 63){
				object->cursorIndex = 0;

				 //switch mode when cursor has reached the end of the second display array
				object->state = setCursorToTop;
				_requestHandled(object);
			}else{
				object->cursorIndex++;
			}
			break;

		default:
			//switch mode
			object->state = setCursorToTop;
			break;
	}
}

void _updateDisplayArray(display_t *object)
{
	uint8_t i  = 0;

	for(i = 0; i<16; i++) {
		object->outputArray[i] = object->outputArrayMirror[i];
	}
	for(i = 0; i<16; i++) {
		object->outputArray[i+32] = object->outputArrayMirror[i+16];
	}
	for(i = 0; i<16; i++) {
		object->outputArray[i+16] = object->outputArrayMirror[i+32];
	}
	for(i = 0; i<16; i++) {
		object->outputArray[i+48] = object->outputArrayMirror[i+48];
	}
}

