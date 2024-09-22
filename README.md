# FastFlow
Reflow Oven Controller

## Hardware

### STM32F411 (Nucleo-64)


## Features

### Oven

#### Heater

HEATER_PWM PA8

#### door open detection

DOOR_CLOSED PC9

#### temperature sesor

use MAX31865 IC

SPI3_MOSI PC12
SPI3_MISO PC11

SPI3_CLK PC10
SPI3_CS_MAX31865 PA15

### Device Interface

#### Serial over USB

USART_RX PA3
USART_TX PA2

### User Interface

#### Rotary encoder with pushbutton

[how to](https://www.youtube.com/watch?v=4ptfrlxA-9E)

ENCODER_A PA6
ENCODER_B PA7

#### 16x2 LCD or equivalent

#### Buzzer

BUZZER_PWM PB6

