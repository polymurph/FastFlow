
#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <stdbool.h>

typedef enum{
	NO_MOVEMENT = 0,
	MOVED_CLOCKWISE,
	MOVED_COUNTERCLOCKWISE,
	MOVED_CLOCKWISE_FAST,
	MOVED_COUNTERCLOCKWISE_FAST
}encoder_movement_t;

void encoder_init();

bool encoder_readPushButton();

encoder_movement_t encoder_read();

#endif // _ENCODER_H_
