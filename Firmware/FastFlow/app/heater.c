#include "heater.h"
#include "pid.h"
#include <stdbool.h>

enum{
	_RAMP_TO_SOAK,
	_SOAK,
	_RAMP_TO_PEAK,
	_RAMP_TO_COOLDOWN
};

static volatile uint16_t _profileTime_s = 0;
static pid_t pid;

static bool _interruptFlag = false;

// local functions declaratrion
void _enableHeaterPWM();
void _dissableHeaterPWM();
void _startTimerInterrupt();
void _stopTimerInterrupt();
void _setPWMdutyCycle(uint32_t dutycycle);
float _readTemp();

// interrupt service routine
void _heaterControllISR();




void heaterInit()
{
	pid_init(&pid, T_s, K_P, K_I, K_D, I_max, I_min, 0xFFFFFFFF, 0)
}

void heaterSetupTemperaturProfile()
{

}

void heaterStart()
{
	_timer = 0;
	_enableHeaterPWM();
	_startTimerInterrupt();
}

void heaterStop()
{
	_dissableHeaterPWM();
	_stopTimerInterrupt();
	_profileTime = 0;
}

void heaterRoutine()
{
	static uint8_t state = _RAMP_TO_SOAK;
	float temp = 0;
	if(_interruptFlag){
		_interruptFlag = false;
		temp = _readTemp();
		switch(state

		){
			case _RAMP_TO_SOAK:
				state++;
				break;
			case _SOAK:
				state++;
				break;
			case _RAMP_TO_PEAK:
				state++;
				break;
			case _RAMP_TO_COOLDOWN:
				state = _RAMP_TO_PEAK;
				break;
			default:
				break;
		}
	}
}

void _enableHeaterPWM()
{

}

void _dissableHeaterPWM()
{

}

void _startTimerInterrupt()
{

}

void _stopTimerInterrupt()
{

}

void _setPWMdutyCycle(uint32_t dutycycle)
{

}

float _readTemp()
{
	return 0;
}

void _heaterControllISR()
{
	_profileTime++;
	_



}
