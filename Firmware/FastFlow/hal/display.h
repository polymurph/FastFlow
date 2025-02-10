/**
 * @file    display.h
 * @brief   display driver containing HDD44780
 *
 * [A more detailed description of what this file contains, its role in the project,
 * and any specific details worth mentioning, such as key functions, structures, or
 * macros declared in the file.]
 *
 * Example usage:
 * @code
 * // Sample code that demonstrates how to use a function or feature in this file.
 * #include "filename.h"
 *
 * int main() {
 *     function_from_header();
 *     return 0;
 * }
 * @endcode
 *
 * @author  Edwin Koch
 * @date    29.09.2024
 *
 * @copyright
 * [Optional: Specify any license, copyright information, or usage guidelines]
 *
 */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>

typedef void(*display_delay)();
typedef void(*display_writePort)(uint8_t);
typedef void(*display_readWrite)(bool);
typedef void(*display_regSelect)(bool);
typedef void(*display_enable)(bool);



typedef enum{
	SET_CURSOR_POSITION,
	UPDATE_DISPLAY,
	SET_CURSOR_MODE,
	SET_FUNCTION
}display_cmd_t;

enum cursor_modes_e
{
	INVISIBLE,
	VISIBLE,
	BLINK,
	VISIBLE_BLINK,
	AUTO_INCREMENT
};

typedef enum{
	setCursorToTop,
	firstLines,
	setCursorToMiddle,
	secondLines
}displayStateMachine_modes_t;

typedef struct{
	display_delay delay;
	display_writePort writePort;
	display_readWrite readWrite;
	display_enable enable;
	display_regSelect regSelect;
	char  outputArrayMirror[64];
	char outputArray[64];
	volatile uint8_t cursorRow;
	volatile uint8_t cursorIndex;
	displayStateMachine_modes_t state;
	uint8_t requestPipeline[3][32];
	uint8_t writeIndex;
	uint8_t workIndex;
}display_t;

/**
 * @brief Initialize display
 * To use this function display_update() must be active.
 *
 */
void display_init(
		display_t *object,
		display_delay delay,
		display_writePort writePort,
		display_readWrite readWrite,
		display_enable enable,
		display_regSelect regSelect);

/**
 * display update routine
 *
 * It needs to be called periodically by e.g. interrupt service routine, RTOS thread, etc.
 * The period is not allowed to be shorter than 40us.
 * All the requests placed by @ref display_request and @ref display_print are handeled
 * here asynchronously from the time of request. This function should not
 */
bool display_updateRoutine(display_t *object);

void display_request(
		display_t *object,
		display_cmd_t cmd,
		uint8_t var1,
		uint8_t var2);

void display_print(
		display_t *object,
		char* text,
		uint8_t len,
		uint8_t row,
		uint8_t column);

#endif //_DISPLAY_H_
