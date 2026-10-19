#ifndef hardware_init_h
#define hardware_init_h

#include "dht.h"
#include <stdio.h>
#include <time.h>
#include "esp_err.h"
#include "driver/i2c_master.h"
#include "ssd1306.h"
#include "ds1302.h"
#include "pinos.h"

// Torna os handles acessíveis para os outros arquivos .c
extern i2c_master_bus_handle_t g_i2c_bus;
extern ssd1306_handle_t g_oled;
extern ds1302_t g_rtc_dev;

// Função responsável por ligar todos os periféricos
esp_err_t hardware_init(void);

esp_err_t dht_ler_dados(void);

esp_err_t botoes_init(void);
void botoes_ler(void);

#endif