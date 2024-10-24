#include "ui.h"

#include <stdbool.h>

#include "hardware.h"

#include "tinyfsm.h"

#include "encoder.h"

ui_event_t eventBuffer;

fsm_t ui_fsm;

display_t *displayPtr;

uint8_t rowindex = 0;
uint8_t columnIndex = 0;

bool blinkState = false;

char clear[] = "                                                                ";
char menu[] = " r_s     T_r     T_s     r_c     t_s     T_f     r_r     start  ";

// state declaration
void _state_listMenu();

// action declaration
void _action_initSelectArrow();
void _action_moveSelectArrowUp();
void _action_moveSelectArrowDown();


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

// action implementation
void _action_initSelectArrow()
{
	display_request(displayPtr, SET_CURSOR_MODE, INVISIBLE, 0);
	display_print(displayPtr, ">", 1, rowindex, 0);
	display_request(displayPtr, SET_CURSOR_POSITION, rowindex, 0);
}

void _action_moveSelectArrowUp()
{
	display_print(displayPtr, " ", 1, rowindex & 0x03, columnIndex);
	rowindex--;
	rowindex &= 0x07;
	if(rowindex >0x03) {
		columnIndex = 8;
	} else {
		columnIndex = 0;
	}
	display_print(displayPtr, ">", 1, rowindex & 0x03, columnIndex);
	display_request(displayPtr, SET_CURSOR_POSITION, rowindex, columnIndex);
}

void _action_moveSelectArrowDown()
{
	display_print(displayPtr, " ", 1, rowindex & 0x03, columnIndex);
	rowindex++;
	rowindex &= 0x07;
	if(rowindex >0x03) {
		columnIndex = 8;
	} else {
		columnIndex = 0;
	}
	display_print(displayPtr, ">", 1, rowindex & 0x03, columnIndex);
	display_request(displayPtr, SET_CURSOR_POSITION, rowindex & 0x03, columnIndex);
}


