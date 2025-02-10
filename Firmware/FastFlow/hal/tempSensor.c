#include "tempSensor.h"
#include <stdbool.h>
#include <stdint.h>

#include "hardware.h"
#include "main.h"
#include "gpio.h"
#include "spi.h"

#include "max31865.h"

static max31865_t max31865_obj;

void _chipSelect(bool select);
uint8_t _spiTRX(uint8_t data);
void _delayChargeTime();
void _delayConversionTime();
void _highTempFault();
void _lowTempFault();


void tempSensor_inti()
{
	_chipSelect(false);

	max31865_init(
			&max31865_obj,
			_chipSelect,
			_spiTRX,
			_delayChargeTime,
			_delayConversionTime,
			_highTempFault,
			_lowTempFault,
			100,
			430,
			0x0000,
			0xFFFF,
			true,
			true);
}

float tempSensor_ReadTemperatureCelsius()
{
	return max31865_readCelsius(&max31865_obj);
}

uint16_t tempSensor_ReadADC()
{
	return max31865_readADC(&max31865_obj);
}

void _chipSelect(bool select)
{
	HAL_GPIO_WritePin(
			SPI3_CS_MAX31865_GPIO_Port,
			SPI3_CS_MAX31865_Pin,
			!select);
}

uint8_t _spiTRX(uint8_t data)
{
	uint8_t buff = 0;
	HAL_SPI_TransmitReceive(&hspi3, &data, &buff, 1, 0);
	return buff;
}

void _delayChargeTime()
{
	uint32_t tNow = hw_getTick();
	while(hw_getTick() != (tNow + 10));
}

void _delayConversionTime()
{
	uint32_t tNow = hw_getTick();
	while(hw_getTick() != (tNow + 10));
}

void _highTempFault()
{

}

void _lowTempFault()
{

}

