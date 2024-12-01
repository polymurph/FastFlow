#include "heater.h"
#include "pid.h"
#include <stdbool.h>
#include <stdint.h>

enum{
	_INIT,
	_RAMP_TO_SOAK_STARTUP,
	_RAMP_TO_SOAK,
	_SOAK,
	_RAMP_TO_PEAK,
	_RAMP_TO_COOLDOWN,
	_DONE
};

static volatile uint16_t _profileTime_s = 0;
static pid_t pid;

static volatile bool _interruptFlag = false;

float _soakTemp = 0;
uint16_t _soakTime = 0;
uint16_t _updateperiode = 0;
float _rampRate;
float _rampIncrement;
float _reflowTemp;
float _cooldownRate;

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
	pid_init(&pid, T_s, K_P, K_I, K_D, I_max, I_min, 0xFFFFFFFF, 0);
}

void heaterSetupTemperaturProfile()
{
	_rampIncrement = (float)_updateperiode * _rampRate;
	_cooldwonIncrement = (float)_updateperiode * _cooldownRate;
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
	float controllOnTemp = 0.7 * _soakTemp;
	static uint16_t _soakTimeLimit = 0;
	float Tset = 0;


	if(_interruptFlag){
		_interruptFlag = false;
		temp = _readTemp();

		switch(state){
			case _INIT:
				_setPWMdutyCycle(0xFFFF);
				state++;
				break;

			case _RAMP_TO_SOAK_STARTUP:
				if(temp >= controllOnTemp){
					state++;
				}
				break;

			case _RAMP_TO_SOAK:
				_setPWMdutyCycle(pid_update(&pid, _soakTemp - temp));

				if(temp < _soakTemp + 0.5 ||  temp > _soakTemp - 0.5){
					state++;
					_soakTimeLimit = _profileTime_s + _soakTime;
				}
				break;

			case _SOAK:
				_setPWMdutyCycle(pid_update(&pid, _soakTemp - temp));
				if(_profileTime_s >= _soakTimeLimit){
					Tset = _soakTemp;
					state++;
				}
				break;

			case _RAMP_TO_PEAK:
				_setPWMdutyCycle(pid_update(&pid, Tset - temp));
				if(Tset >= _reflowTemp){
					state++;
				}else{
					Tset += _rampIncrement;
				}
				break;

			case _RAMP_TO_COOLDOWN:
				_setPWMdutyCycle(pid_update(&pid, Tset - temp));
				if(Tset < _reflowTemp){
					_stopTimerInterrupt();
					_setPWMdutyCycle(0);
					_dissableHeaterPWM();
					state++;
				}else{
					Tset += _cooldwonIncrement;
				}
				break;

			case _DONE:
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
	_interruptFlag = true;
}
