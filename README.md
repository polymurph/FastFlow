# FastFlow
Reflow Oven Controller

## Hardware

### STM32F411 (Nucleo-64)

[pinout](https://os.mbed.com/platforms/ST-Nucleo-F411RE/)

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

ENCODER_BUTTON PC4

#### 16x244 LCD or equivalent

[1604A](https://www.farnell.com/datasheets/50577.pdf)

EA W164B-NLW

<img src=".\docu\pics\display_1604-A.JPG" width="400" height="auto">

<img src=".\docu\pics\pinout_1604-A.JPG" width="400" height="auto">

#### Buzzer

BUZZER_PWM PB6

[PWM](https://www.youtube.com/watch?v=k1jHQ7oW4Uw)
#### LED

## Info

[DMA](https://www.youtube.com/watch?v=pMbMRMbdOX8)


