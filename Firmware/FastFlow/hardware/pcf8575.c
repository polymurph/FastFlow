#include "pcf8575.h"
#include <stddef.h>

void _read(pcf8575_t *object, uint8_t *data, uint8_t len);
void _write(pcf8575_t *object, uint8_t *data, uint8_t len);
void _updatePorts(pcf8575_t *object);

void pcf8575_init(
		pcf8575_t *object,
		uint8_t i2cAddress,
		i2c_read_t i2cRead,
		i2c_write_t i2cWrite,
		uint8_t port0,
		uint8_t port1)
{
	// init objects
	object->address = (pcf8575_I2CaddrBase | (i2cAddress & pcf8575_I2CaddrMask)) << 1;
	object->_i2cRead = i2cRead;
	object->_i2cWrite = i2cWrite;
	object->_port0Buf = port0;
	object->_port1Buf = port1;

	// init hardware
	_updatePorts(object);
}

void pcf8575_writePort(
		pcf8575_t *object,
		pcf8575_IOport_t port,
		uint8_t data)
{
	if(port == PCF8575_IOPORT_0) {
		object->_port0Buf = data;

	} else {
		object->_port1Buf = data;
	}
	_updatePorts(object);
}

uint8_t pcf8575_readPort(
		pcf8575_t *object,
		pcf8575_IOport_t port)
{
	uint8_t data[2] = {0};

	_read(object, data, 2);

	if(port == PCF8575_IOPORT_0){
		return data[0];
	}
	return data[1];
}

void pcf8575_writePin(
		pcf8575_t *object,
		pcf8575_IOport_t port,
		uint8_t pin,
		bool state)
{
	uint8_t *pBuf = NULL;
	// only pin numbers 0 to 7 allowed
	pin &= 0x07;

	if(port == PCF8575_IOPORT_0){
		pBuf = &(object->_port0Buf);
	} else {
		pBuf = &(object->_port1Buf);
	}

	*pBuf &= ~(1 << pin);

	if(state){
		*pBuf |= state << pin;
	}

	_updatePorts(object);
}

bool pcf8575_readPin(
		pcf8575_t *object,
		pcf8575_IOport_t port,
		uint8_t pin)
{
	uint8_t data[2] = {0};

	// only pin numbers 0 to 7 allowed
	pin &= 0x07;

	_read(object, data, 2);

	if(port == PCF8575_IOPORT_0){
		return data[0] & 1 << pin;
	}
	return data[1] & 1 << pin;
}

void pcf8575_togglePin(
		pcf8575_t *object,
		pcf8575_IOport_t port,
		uint8_t pin)
{
	uint8_t *pBuf = NULL;
	// only pin numbers 0 to 7 allowed
	pin &= 0x07;

	if(port == PCF8575_IOPORT_0){
		pBuf = &(object->_port0Buf);
	} else {
		pBuf = &(object->_port1Buf);
	}

	*pBuf ^= (1 << pin);
	_updatePorts(object);
}

void _read(pcf8575_t *object, uint8_t *data, uint8_t len)
{
	object->_i2cRead(object->address, data, len);
}

void _write(pcf8575_t *object, uint8_t *data, uint8_t len)
{
	object->_i2cWrite(object->address, data, len);
}

void _updatePorts(pcf8575_t *object)
{
	uint8_t data[2] = {object->_port0Buf, object->_port1Buf};
	_write(object, data, 2);
}
