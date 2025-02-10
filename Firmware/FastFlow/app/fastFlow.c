#include "fastFlow.h"
#include <stdint.h>
#include <stdbool.h>



#include "usart.h"

#include "tim.h"
#include "i2c.h"
#include "gpio.h"


#include "ui.h"

#include "tinyfsm.h"
#include "../hal/buzzer.h"
#include "../hal/encoder.h"
#include "../hal/hardware.h"

static fsm_t fastFlowFSM;



// states
void _state_menu();


// actions
void _action_init();

//void _i2cWrite(uint8_t address, uint8_t* data,uint8_t len);
//void _i2cRead(uint8_t address, uint8_t* data,uint8_t len);


void fastFlow_init()
{
	fsmInitSingleThreaded(&fastFlowFSM, _state_menu, _action_init, fsmNoAction);
}


void fastFlow_run()
{
	/*
	uint8_t helloWorld_msg[] = "Hello, World!\n\r";
	uint8_t turnedRight_msg[] = "Turned Right!\n\r";
	uint8_t turnedLeft_msg[] = "Turned Left!\n\r";
	uint8_t turnedRightFast_msg[] = "Turned Right Fast!\n\r";
	uint8_t turnedLeftFast_msg[] = "Turned Left Fast!\n\r";
	uint8_t buttonPressed_msg[] = "Button pressed!\n\r";

	uint8_t data_0[2] = {0xFF,0xFF};
	uint8_t data_1[2] = {0x00,0x00};

	uint8_t dump[2] = {0};
	*/

	//HAL_UART_Transmit(&huart2, helloWorld_msg, sizeof(helloWorld_msg) - 1, 1000);




	while(1) fsmRun(&fastFlowFSM);

}

// states implementation

void _state_menu()
{
	//const uint32_t c_uiRefreshRateNtick = 150;
	const uint32_t c_uiRefreshRateNtick = 500;

	const uint32_t c_LEDRefreshRateNtick = 500;
	const uint32_t c_displayRefreshRateNtick = 1;

	static uint32_t uiLastTick = 0;
	static uint32_t LEDLastTick = 0;
	static uint32_t displayLastTick = 0;

	uint32_t tickNow = hw_getTick();

	if(tickNow - uiLastTick > c_uiRefreshRateNtick){
		uiLastTick = tickNow;
		ui_updateRoutine();
	}


	if(tickNow - LEDLastTick > c_LEDRefreshRateNtick){
		LEDLastTick = tickNow;
		ui_toggleLED();
	}

	if(tickNow - displayLastTick > c_displayRefreshRateNtick){
		displayLastTick = tickNow;
		ui_displayUpdateRoutine();
	}
}

// actions implementation

void _action_init()
{
	char initMSG[] = "Initializing Fast Flow...\n\r";
	char doneMSG[] = "...done!\n\r";

	HAL_UART_Transmit(&huart2, initMSG, sizeof(initMSG) - 1, 1000);

	hw_init();

	ui_init();
	HAL_UART_Transmit(&huart2, doneMSG, sizeof(doneMSG) - 1, 1000);
}
