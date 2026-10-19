#ifndef pinos_h
#define pinos_h

#include "driver/gpio.h"
#include "driver/i2c_master.h"

// ---Display (I2C)---
#define I2C_SDA GPIO_NUM_21
#define I2C_SCL GPIO_NUM_22

// ---Relógio DS1302---
#define DS1302_CLK GPIO_NUM_18
#define DS1302_IO GPIO_NUM_19
#define DS1302_CE GPIO_NUM_5

// ---DHT11---
#define DHT_GPIO_PIN GPIO_NUM_4

// ---Válvula---
#define VS GPIO_NUM_15

// ---Botões---
#define BT_REGA GPIO_NUM_2

#endif