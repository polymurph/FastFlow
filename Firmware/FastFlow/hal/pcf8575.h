#ifndef _PCF8575_H_
#define _PCF8575_H_

#include <stdint.h>
#include <stdbool.h>

static const uint8_t pcf8575_I2CaddrBase = 0x20;
static const uint8_t pcf8575_I2CaddrMask = 0x07;

typedef void(*i2c_read_t)(uint8_t, uint8_t*, uint8_t);
typedef void(*i2c_write_t)(uint8_t, uint8_t*, uint8_t);

typedef enum{
	PCF8575_IOPORT_0,
	PCF8575_IOPORT_1
}pcf8575_IOport_t;

typedef struct{
	uint8_t address;
	i2c_read_t _i2cRead;
	i2c_write_t _i2cWrite;
	uint8_t _port0Buf;
	uint8_t _port1Buf;
}pcf8575_t;

void pcf8575_init(
		pcf8575_t *object,
		uint8_t i2cAddress,
		i2c_read_t i2cRead,
		i2c_write_t i2cWrite,
		uint8_t port0,
		uint8_t port1);

void pcf8575_writePort(
		pcf8575_t *object,
		pcf8575_IOport_t port,
		uint8_t data);

uint8_t pcf8575_readPort(
		pcf8575_t *object,
		pcf8575_IOport_t port);

void pcf8575_writePin(
		pcf8575_t *object,
		pcf8575_IOport_t port,
		uint8_t pin,
		bool state);

bool pcf8575_readPin(
		pcf8575_t *object,
		pcf8575_IOport_t port,
		uint8_t pin);

void pcf8575_togglePin(
		pcf8575_t *object,
		pcf8575_IOport_t port,
		uint8_t pin);

#endif // _PCF8575_H_
