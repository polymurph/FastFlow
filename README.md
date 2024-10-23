# FastFlow
Reflow Oven Controller

## Reflow

[Reflow Process](https://madpcb.com/reflow-soldering/)

![pic](https://madpcb.com/wp-content/uploads/2020/10/Reflow-Soldering-Reflow-Profile.png)

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

### DMA

[DMA](https://www.youtube.com/watch?v=pMbMRMbdOX8)


### I2C

init problem

inside the function ```HAL_I2C_MspInit``` provided by the
Stm Cube MX the following code must be executed at the end
of the program in the user code section.

```c
__HAL_RCC_I2C1_CLK_ENABLE();
/* USER CODE BEGIN I2C1_MspInit 1 */
__HAL_RCC_I2C1_FORCE_RESET();
HAL_Delay(100);
__HAL_RCC_I2C1_RELEASE_RESET();
```

[source1](http://www.sonsivri.to/forum/index.php?topic=62967.0)
[source2](https://community.st.com/t5/stm32-mcus-products/stm32f4-i2c-issues-solved/td-p/526774)
[source3](https://github.com/wokwi/wokwi-features/issues/744)
