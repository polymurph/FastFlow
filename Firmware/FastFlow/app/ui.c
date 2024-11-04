#include "ui.h"

#include <stdbool.h>

#include "hardware.h"

#include "tinyfsm.h"

#include "encoder.h"

#include "usart.h"

ui_event_t eventBuffer;

fsm_t ui_fsm;

display_t *displayPtr;

uint8_t rowIndex = 0;
uint8_t columnIndex = 0;

bool blinkState = false;

char clear[] = "                                                                ";
char menu[] = " r_s     T_r     T_s     r_c     t_s     T_f     r_r     start  ";

// state declaration
void _state_listMenu();
void _state_set_r_s();
void _state_set_T_s();
void _state_set_t_s();
void _state_set_r_r();
void _state_set_T_r();
void _state_set_r_c();
void _state_set_T_f();


// action declaration
void _action_initSelectArrow();
void _action_moveSelectArrowUp();
void _action_moveSelectArrowDown();
void _action_turnOnCursorBlink();
void _action_turnOffCursorBlink();

// local fucntions
bool _buttonPressed();
void _displayPrintNumber(display_t *displayObject, uint8_t number, uint8_t row, uint8_t column);
void _encoderUpdateparameter(display_t *displayObject, uint8_t *param, uint8_t row, uint8_t column);

void ui_init()
{

	//encoder_init();
	displayPtr = hw_getDisplayobjectByPtr();


	display_print(displayPtr, menu, sizeof(menu), 0, 0);
	display_request(displayPtr, SET_CURSOR_POSITION, 0, 0);


	while(display_updateRoutine(displayPtr));


	fsmInitSingleThreaded(&ui_fsm, _state_listMenu, _action_initSelectArrow, fsmNoAction);
}

void ui_updateRoutine()
{
	fsmRun(&ui_fsm);
}

void ui_handleEvent(ui_event_t event)
{
	eventBuffer = event;
}

void ui_displayUpdateRoutine()
{
	display_updateRoutine(displayPtr);
}

void ui_toggleLED()
{
	hw_toggleLED();
}

// stat implementation

void _state_listMenu()
{
	/*
	static bool buttonOldState = false;
	bool buttonState = encoder_readPushButton();

	if(buttonState && (buttonState != buttonOldState)){
		if(blinkState){
			display_request(displayPtr, SET_CURSOR_MODE, VISIBLE_BLINK, 0);
			blinkState = false;
		} else {
			display_request(displayPtr, SET_CURSOR_MODE, INVISIBLE, 0);
			blinkState = true;
		}
	}

	buttonOldState = buttonState;
	*/

	if(_buttonPressed()){
		switch(rowIndex){

			case 0x00:
				fsmTransitionState(&ui_fsm, _state_set_r_s, _action_turnOnCursorBlink);
				return;

			case 0x01:
				fsmTransitionState(&ui_fsm, _state_set_T_s, _action_turnOnCursorBlink);
				return;

			case 0x02:
				fsmTransitionState(&ui_fsm, _state_set_t_s, _action_turnOnCursorBlink);
				return;

			case 0x03:
				fsmTransitionState(&ui_fsm, _state_set_r_r, _action_turnOnCursorBlink);
				return;

			case 0x04:
				fsmTransitionState(&ui_fsm, _state_set_T_r, _action_turnOnCursorBlink);
				return;

			case 0x05:
				fsmTransitionState(&ui_fsm, _state_set_r_c, _action_turnOnCursorBlink);
				return;

			case 0x06:
				fsmTransitionState(&ui_fsm, _state_set_T_f, _action_turnOnCursorBlink);
				return;

			case 0x07:
				//fsmTransitionState(&ui_fsm, _state_set_t_s, _action_turnOnCursorBlink);
				break;

			default:
				break;
		}
	}


	switch(encoder_read()){
		case NO_MOVEMENT:
			return;

		case MOVED_CLOCKWISE:
			_action_moveSelectArrowDown();
			break;

		case MOVED_COUNTERCLOCKWISE:
			_action_moveSelectArrowUp();
			break;


		default:
			break;
	}
}

void _state_set_r_s()
{
	static uint8_t numb = 111;
	if(_buttonPressed()){
		fsmTransitionState(&ui_fsm, _state_listMenu, _action_turnOffCursorBlink);
		return;
	}

	_encoderUpdateparameter(displayPtr, &numb, 0, 5);

}

void _state_set_T_s()
{
	static uint8_t numb = 111;
	if(_buttonPressed()){
		fsmTransitionState(&ui_fsm, _state_listMenu, _action_turnOffCursorBlink);
		return;
	}
	_encoderUpdateparameter(displayPtr, &numb, 1, 5);
}

void _state_set_t_s()
{
	static uint8_t numb = 111;
	if(_buttonPressed()){
		fsmTransitionState(&ui_fsm, _state_listMenu, _action_turnOffCursorBlink);
		return;
	}
	_encoderUpdateparameter(displayPtr, &numb, 2, 5);
}

void _state_set_r_r()
{
	static uint8_t numb = 111;
	if(_buttonPressed()){
		fsmTransitionState(&ui_fsm, _state_listMenu, _action_turnOffCursorBlink);
		return;
	}
	_encoderUpdateparameter(displayPtr, &numb, 3, 5);
}

void _state_set_T_r()
{
	static uint8_t numb = 111;
	if(_buttonPressed()){
		fsmTransitionState(&ui_fsm, _state_listMenu, _action_turnOffCursorBlink);
		return;
	}
	_encoderUpdateparameter(displayPtr, &numb, 0, 13);
}

void _state_set_r_c()
{
	static uint8_t numb = 111;
	if(_buttonPressed()){
		fsmTransitionState(&ui_fsm, _state_listMenu, _action_turnOffCursorBlink);
		return;
	}
	_encoderUpdateparameter(displayPtr, &numb, 1, 13);
}

void _state_set_T_f()
{
	static uint8_t numb = 111;
	if(_buttonPressed()){
		fsmTransitionState(&ui_fsm, _state_listMenu, _action_turnOffCursorBlink);
		return;
	}
	_encoderUpdateparameter(displayPtr, &numb, 2, 13);
}

// action implementation
void _action_initSelectArrow()
{
	display_request(displayPtr, SET_CURSOR_MODE, INVISIBLE, 0);
	display_print(displayPtr, ">", 1, rowIndex, 0);
	display_request(displayPtr, SET_CURSOR_POSITION, rowIndex, 0);
}

void _action_moveSelectArrowUp()
{
	uint8_t data[3] = {0,'\n','\r'};
	display_print(displayPtr, " ", 1, rowIndex & 0x03, columnIndex);
	rowIndex--;
	rowIndex &= 0x07;
	if(rowIndex >0x03) {
		columnIndex = 8;
	} else {
		columnIndex = 0;
	}
	data[0] = rowIndex & 0x03 | 0x30;
	HAL_UART_Transmit(&huart2, &data, 3, 1000);
	data[0] = columnIndex >>3 | 0x30;
	HAL_UART_Transmit(&huart2, &data, 3, 1000);
	display_print(displayPtr, ">", 1, rowIndex & 0x03, columnIndex);
	display_request(displayPtr, SET_CURSOR_POSITION, rowIndex, columnIndex);
}

void _action_moveSelectArrowDown()
{
	uint8_t data[3] = {0,'\n','\r'};
	display_print(displayPtr, " ", 1, rowIndex & 0x03, columnIndex);
	rowIndex++;
	rowIndex &= 0x07;
	if(rowIndex >0x03) {
		columnIndex = 8;
	} else {
		columnIndex = 0;
	}
	data[0] = rowIndex & 0x03 | 0x30;
	HAL_UART_Transmit(&huart2, &data, 3, 1000);
	data[0] = columnIndex >>3 | 0x30;
	HAL_UART_Transmit(&huart2, &data, 3, 1000);
	display_print(displayPtr, ">", 1, rowIndex & 0x03, columnIndex);
	display_request(displayPtr, SET_CURSOR_POSITION, rowIndex & 0x03, columnIndex);
}


void _action_turnOnCursorBlink()
{
	display_request(displayPtr, SET_CURSOR_MODE, VISIBLE_BLINK, 0);
}

void _action_turnOffCursorBlink()
{
	display_request(displayPtr, SET_CURSOR_MODE, INVISIBLE, 0);
}


// local functions implementeation

bool _buttonPressed()
{
	static bool buttonOldState = false;
	bool buttonState = encoder_readPushButton();

	if(buttonState && (buttonState != buttonOldState)){
		buttonOldState = buttonState;
		return true;
	}

	buttonOldState = buttonState;
	return false;
}

void _displayPrintNumber(display_t *displayObject, uint8_t number, uint8_t row, uint8_t column)
{

	uint8_t buf[3];

	buf[0] = number / 100;
	number -= 100 * buf[0];
	buf[1] = number / 10;
	number -= 10 * buf[1];
	buf[2] = number;

	for(number = 0; number < 3; number++){
		buf[number] |= 0x30;
	}

	display_print(displayObject, buf, 3, row, column);
	// put back curset at original position where the index is
	display_request(displayObject, SET_CURSOR_POSITION, rowIndex & 0x03, columnIndex);
}

void _encoderUpdateparameter(display_t *displayObject, uint8_t *param, uint8_t row, uint8_t column)
{
	switch(encoder_read()){
		case NO_MOVEMENT:
			return;

		case MOVED_CLOCKWISE:
			*param += 1;
			_displayPrintNumber(displayObject, *param, row, column);
			break;

		case MOVED_COUNTERCLOCKWISE:
			*param -= 1;
			_displayPrintNumber(displayObject, *param, row, column);
			break;

		default:
			break;
	}
}


