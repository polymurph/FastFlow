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

/**
 * @brief Initialize display
 * To use this function display_update() must be active.
 *
 */
void display_init();

/**
 * display update routine
 *
 * It needs to be called periodically by e.g. interrupt service routine, RTOS thread, etc.
 * The period is not allowed to be shorter than 40us.
 * All the requests placed by @ref display_request and @ref display_print are handeled
 * here asynchronously from the time of request. This function should not
 */
void display_updateRoutine();

void display_request(
		display_cmd_t cmd,
		uint8_t var1,
		uint8_t var2);

void display_print(
		char* text,
		uint8_t len,
		uint8_t row,
		uint8_t column);


#endif //_DISPLAY_H_
