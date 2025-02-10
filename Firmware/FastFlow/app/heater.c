#include "heater.h"
#include "pid.h"
#include <stdbool.h>
#include <stdint.h>
#include "max31865.h"
#include "spi.h"
#include "hardware.h"

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
static max31865_t max31865;

static volatile bool _interruptFlag = false;

float _soakTemp = 0;
uint16_t _soakTime = 0;
uint16_t _updateperiode = 0;
uint16_t _profileTime;
uint16_t _timer;
float _rampRate;
float _rampIncrement;
float _reflowTemp;
float _cooldownRate;
float _cooldwonIncrement;

// local functions declaratrion
void _enableHeaterPWM();
void _dissableHeaterPWM();
void _startTimerInterrupt();
void _stopTimerInterrupt();
void _setPWMdutyCycle(uint32_t dutycycle);
float _readTemp();

void _chipSelect(bool select);
uint8_t _spiTRX(uint8_t data);
void _delayChargeTime();
void _delayConversionTime();
void _highTHfault();
void _lowTHfault();



// interrupt service routine
void _heaterControllISR();




void heaterInit()
{
	//pid_init(&pid, T_s, K_P, K_I, K_D, I_max, I_min, 0xFFFFFFFF, 0);

	max31865_init(&max31865,
			_chipSelect,
			_spiTRX,
			_delayChargeTime,
			_delayConversionTime,
			_highTHfault,
			_lowTHfault,
			100,
			430,
			0x0000,
			0xFFFF,
			true,
			true);
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

uint32_t heaterGetTemp()
{
	return (uint32_t)max31865_readCelsius(&max31865);
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

void _chipSelect(bool select)
{
	HAL_GPIO_WritePin(SPI3_CS_MAX31865_GPIO_Port, SPI3_CS_MAX31865_Pin, select);
}

uint8_t _spiTRX(uint8_t data)
{
	uint8_t rx;
	HAL_SPI_TransmitReceive(&hspi3, &data, &rx, 1,0);
	return 0;
}

void _delayChargeTime()
{
	uint32_t tNow = hw_getTick();
	while(hw_getTick > tNow + 1000);
}

void _delayConversionTime()
{
	uint32_t tNow = hw_getTick();
	while(hw_getTick > tNow + 1000);
}

void _highTHfault()
{

}

void _lowTHfault()
{

}

void _heaterControllISR()
{
	_profileTime++;
	_interruptFlag = true;
}
