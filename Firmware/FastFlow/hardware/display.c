#include "display.h"

#include <string.h>
#include "gpio.h"

#include "stm32f4xx_hal.h"

static const uint8_t _delay_ms = 10;

typedef enum{
	setCursorToTop,
	firstLines,
	setCursorToMiddle,
	secondLines
}displayStateMachine_modes_t;

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

char _displayOutputArrayMirror[64];
static char _displayOutputArray[64];

uint8_t _writeIndex = 0;
uint8_t _workIndex = 0;

uint8_t _requestPipeline[3][32];

displayStateMachine_modes_t _displayStateMachineMode = setCursorToTop;

display_delay _delay;
display_writePort _writePort;
display_readWrite _readWrite;
display_enable _enable;
display_regSelect _regSelect;

void _setCursorPosition(uint8_t row, uint8_t column);
void _requestHandled();
void _placeRequest(uint8_t mode, uint8_t var1, uint8_t var2);
void _writeCommandToDisplay(uint8_t cmd);
void _writeCharToDisplay(char data);
void _writeToDisplay();
void _updateDisplayArray();


void display_init(
		display_delay delay,
		display_writePort writePort,
		display_readWrite readWrite,
		display_enable enable,
		display_regSelect regSelect)
{
	_delay = delay;
	_writePort = writePort;
	_readWrite = readWrite;
	_enable = enable;
	_regSelect = regSelect;

	_displayCursorRow = 0;
	_displayCursorIndex = 0;

	display_request(SET_FUNCTION, 0, 0);
	//display_request(SET_CURSOR_MODE,INVISIBLE,0);
	display_request(SET_CURSOR_MODE,BLINK,0);
	display_request(SET_CURSOR_MODE,AUTO_INCREMENT,0);
	display_request(UPDATE_DISPLAY,0,0);
}

bool display_updateRoutine()
{
	// check for new job
	if(!(_workIndex != _writeIndex)) return false;

	switch (_requestPipeline[0][_workIndex+1]) {

		case SET_CURSOR_POSITION:
			_setCursorPosition(_requestPipeline[1][_workIndex+1], _requestPipeline[2][_workIndex+1]);
			_requestHandled();
			break;

		case UPDATE_DISPLAY:
			_writeToDisplay();
			break;

		case SET_CURSOR_MODE:
			switch (_requestPipeline[1][_workIndex+1])
			{
				case INVISIBLE:
					_writeCommandToDisplay(CURSOR_MODE_INVISIBLE);
					_requestHandled();
					break;

				case VISIBLE:
					_writeCommandToDisplay(CURSOR_MODE_VISIBLE);
					_requestHandled();
					break;

				case BLINK:
					_writeCommandToDisplay(CURSOR_MODE_BLINK);
					_requestHandled();
					break;

				case VISIBLE_BLINK:
					_writeCommandToDisplay(CURSOR_MODE_BLINK | CURSOR_MODE_INVISIBLE);
					_requestHandled();
					break;

				case AUTO_INCREMENT:
					_writeCommandToDisplay(CURSOR_MODE_AUTO_INCREMENT);
					_requestHandled();
					break;

				default:
					_requestHandled();
					break;
			}
			break;

		case SET_FUNCTION:
			_writeCommandToDisplay(DISPLAY_FUNCTION_SET);
			_requestHandled();
			break;

		default:
		break;
	};
	return true;
}

void display_request(
		display_cmd_t cmd,
		uint8_t var1,
		uint8_t var2)
{
	if(cmd == UPDATE_DISPLAY) {
		_updateDisplayArray();
		_placeRequest(SET_CURSOR_POSITION, 0, 0);
		_placeRequest(UPDATE_DISPLAY, 0, 0);
		_placeRequest(SET_CURSOR_POSITION, 0, 15);
	} else {
		_placeRequest(cmd,var1,var2);
	}
}

void display_print(
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

	memcpy(&_displayOutputArrayMirror[index],text,len);

	display_request(SET_CURSOR_POSITION,row,column);
	display_request(UPDATE_DISPLAY,0,0);
}

void _setCursorPosition(uint8_t row, uint8_t column)
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
	_writeCommandToDisplay((position | DD_RAM_ADDR_SET));
}

void _requestHandled()
{
	_workIndex++;
	_workIndex &= 0x1F;
}

void _placeRequest(uint8_t mode, uint8_t var1, uint8_t var2)
{
	_writeIndex++;
	_writeIndex &= 0x1F;	//overflow reset

	_requestPipeline[0][_writeIndex] = mode;
	_requestPipeline[1][_writeIndex] = var1;
	_requestPipeline[2][_writeIndex] = var2;
}

void _writeCommandToDisplay(uint8_t cmd)
{
#if 0
	uint8_t i=0;

	//while(read_busy_pin() & PIN_BUSY);
	uint8_t i=0;

	CLRBIT(PORTB,DISPLAY_SIGNAL_RS);		//clear register select pin

	CLRBIT(PORTB,DISPLAY_SIGNAL_RW);		//set mode write to display

	SETBIT(PORTB,DISPLAY_SIGNAL_ENABLE);	//set enable pin

	for(i=0 ; i< 100/*42,43,44,45*/;i++){}

	PORTD = (c);

	for(i=0 ; i< 100/*42,43,44,45*/;i++){}

	CLRBIT(PORTB,DISPLAY_SIGNAL_ENABLE);	//clear enable pin

	for(i=0 ; i< 50/*42,43,44,45*/;i++){}

	SETBIT(PORTB,DISPLAY_SIGNAL_RS);		//set resgister select pin

	for(i=0 ; i< 50/*42,43,44,45*/;i++){}
#endif
	//while(read_busy_pin() & PIN_BUSY);

	//clear register select pin
	//HAL_GPIO_WritePin(DISPLAY_REGISTER_SELECT_GPIO_Port, DISPLAY_REGISTER_SELECT_Pin, 0);
	_regSelect(false);

	//set mode write to display
	//HAL_GPIO_WritePin(DISPLAY_READ_WRITE_GPIO_Port, DISPLAY_READ_WRITE_Pin, 0);
	_readWrite(false);

	//set enable pin
	//HAL_GPIO_WritePin(DISPLAY_ENABLE_GPIO_Port, DISPLAY_ENABLE_Pin, 1);
	_enable(true);

	//for(i=0 ; i< 100/*42,43,44,45*/;i++){}
	//HAL_Delay(_delay_ms);
	_delay();

	//GPIOB->ODR &= 0xF403;
	//GPIOB->ODR |= (cmd & 0x07) | ((cmd & 0x08) << 10) | ((cmd & 0xF0) << 12);
	_writePort(cmd);

	//for(i=0 ; i< 100/*42,43,44,45*/;i++){}
	//HAL_Delay(_delay_ms);
	_delay();

	//clear enable pin
	//HAL_GPIO_WritePin(DISPLAY_ENABLE_GPIO_Port, DISPLAY_ENABLE_Pin, 0);
	_enable(false);

	//for(i=0 ; i< 50/*42,43,44,45*/;i++){}
	//HAL_Delay(_delay_ms);
	_delay();

	//set resgister select pin
	//HAL_GPIO_WritePin(DISPLAY_REGISTER_SELECT_GPIO_Port, DISPLAY_REGISTER_SELECT_Pin, 1);
	_regSelect(true);

	//for(i=0 ; i< 50/*42,43,44,45*/;i++){}
	//HAL_Delay(_delay_ms);
	_delay();
}

void _writeCharToDisplay(char data)
{

	//while(read_busy_pin() & PIN_BUSY);
	uint8_t i=0;

	//set enable pin
	//HAL_GPIO_WritePin(DISPLAY_ENABLE_GPIO_Port, DISPLAY_ENABLE_Pin, 1);
	_enable(true);


	//for(i=0 ; i< 50;i++){}
	//HAL_Delay(_delay_ms);
	_delay();

	//set mode write to display
	//HAL_GPIO_WritePin(DISPLAY_READ_WRITE_GPIO_Port, DISPLAY_READ_WRITE_Pin, 0);
	_readWrite(false);

	//asm volatile ("nop");

	//for(i=0 ; i< 100;i++){}
	//HAL_Delay(_delay_ms);
	_delay();

	//GPIOB->ODR &= 0xF403;
	//GPIOB->ODR |= (data & 0x07) | ((data & 0x08) << 10) | ((data & 0xF0) << 12);
	_writePort(data);

	//for(i=0 ; i< 100;i++){}
	//HAL_Delay(_delay_ms);
	_delay();

	//clear enable pin
	//HAL_GPIO_WritePin(DISPLAY_ENABLE_GPIO_Port, DISPLAY_ENABLE_Pin, 0);
	_enable(false);

	//for(i=0 ; i< 50;i++){}
	//HAL_Delay(1);
	_delay();
#if 0
	//while(read_busy_pin() & PIN_BUSY);
	uint8_t i=0;

	SETBIT(PORTB,PINB0);		//set enable pin

	for(i=0 ; i< 50;i++){}

	CLRBIT(PORTB,PINB1); //set mode write to display

	//asm volatile ("nop");

	for(i=0 ; i< 100;i++){}

	PORTD = c;

	for(i=0 ; i< 100;i++){}

	CLRBIT(PORTB,PINB0);	//clear enable pin

	for(i=0 ; i< 50;i++){}
#endif
}

void _writeToDisplay()
{
	switch(_displayStateMachineMode){
		case setCursorToTop:
			//cursor set to the first position in row 0 (first position of the first display array)
			_writeCommandToDisplay(0x00|DD_RAM_ADDR_SET);

			//switch mode
			_displayStateMachineMode = firstLines;

			//reset display_cursor_index
			_displayCursorIndex = 0;
			break;

		case firstLines:
			//write first display array
			_writeCharToDisplay(_displayOutputArray[_displayCursorIndex]);
			if (_displayCursorIndex == 32){
				 //switch mode when cursor has reached the end of the first display array
				_displayStateMachineMode = setCursorToMiddle;
			}else{
				_displayCursorIndex++;
			}
			break;

		case setCursorToMiddle:
			//cursor set to the first position in row 1 (first position of the second display array)
			_writeCommandToDisplay(0x40|DD_RAM_ADDR_SET);

			//switch mode
			_displayStateMachineMode = secondLines;
			break;

		case secondLines:
			//write second display array
			_writeCharToDisplay(_displayOutputArray[_displayCursorIndex]);
			if (_displayCursorIndex == 63){
				_displayCursorIndex = 0;

				 //switch mode when cursor has reached the end of the second display array
				_displayStateMachineMode = setCursorToTop;
				_requestHandled();
			}else{
				_displayCursorIndex++;
			}
			break;

		default:
			//switch mode
			_displayStateMachineMode = setCursorToTop;
			break;
	}
}

void _updateDisplayArray()
{
	uint8_t i  = 0;

	for(i = 0; i<16; i++) {
		_displayOutputArray[i] = _displayOutputArrayMirror[i];
	}
	for(i = 0; i<16; i++) {
		_displayOutputArray[i+32] = _displayOutputArrayMirror[i+16];
	}
	for(i = 0; i<16; i++) {
		_displayOutputArray[i+16] = _displayOutputArrayMirror[i+32];
	}
	for(i = 0; i<16; i++) {
		_displayOutputArray[i+48] = _displayOutputArrayMirror[i+48];
	}
}

