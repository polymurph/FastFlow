
#ifndef _UI_H_
#define _UI_H_

#include "display.h"

typedef enum{
	UI_NO_EVENT,
	UI_EV_ENCODER_TURNED_CC,
	UI_EV_ENCODER_TURNED_CCW,
	UI_EV_ENCODER_BUTTON_PUSHED
}ui_event_t;

void ui_init(display_t *displaObj);

void ui_updateRoutine();

void ui_handleEvent(ui_event_t event);


#endif // _UI_H_
