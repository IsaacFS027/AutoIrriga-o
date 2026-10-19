#include <stdio.h>
#include <time.h>
#include "pinos.h"
#include "icons.h"
#include "variaveis.h"
#include "hardware_init.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

#include "driver/i2c_master.h"
#include "ssd1306.h"

#include "ds1302.h"

void app_main(void)
{
    ESP_ERROR_CHECK(hardware_init());
    struct tm time = {0};
    // receba
    /*struct tm tempo_inicial = {
        .tm_year = 2026 - 1900, // Ano atual (2026)
        .tm_mon  = 8,           // Setembro (Mês 0 a 11, onde 8 = Setembro)
        .tm_mday = 20,          // Dia do mês
        .tm_hour = 1,           // Hora
        .tm_min  = 34,          // Minuto
        .tm_sec  = 0            // Segundo
    };
    ds1302_set_time(&g_rtc_dev, &tempo_inicial);*/

    char buffer[32];

    ssd1306_display_text(g_oled, 1, " Irrigacao Auto ", false);
    ssd1306_display_rectangle(g_oled, 0, 0, 127, 20, false);

    while (1)
    {
        if (contador_dht ++ >= 10) {
        dht_ler_dados();
        contador_dht = 0;
        }

        snprintf(buffer, sizeof(buffer), "Temp: %.1f", g_temperatura);
        ssd1306_display_text(g_oled, 3, buffer, false);
        ssd1306_set_bitmap(g_oled, 80, 24, icon_celsius, 8, 8, false);

        snprintf(buffer, sizeof(buffer), "Humid: %d%%", g_humidade);
        ssd1306_display_text(g_oled, 4, buffer, false);
        ssd1306_set_bitmap(g_oled, 80, 32, icon_gota, 8, 8, false);

        if (ds1302_get_time(&g_rtc_dev, &time) == ESP_OK) {
        snprintf(buffer, sizeof(buffer), "Horas: %02d:%02d:%02d", time.tm_hour, time.tm_min, time.tm_sec);
        ssd1306_display_text(g_oled, 5, buffer, false);
        } else {
            ssd1306_display_text(g_oled, 5, "Horas: --:--:--", false);
        }

        if (time.tm_hour == 8 && time.tm_min == 0 && time.tm_sec == 0)
        {
            g_irrigacao08hr = 1;
            g_bomba_ligada = true;
            gpio_set_level(VS, 1);
            g_tempo_inicio_bomba = esp_timer_get_time();
        } else if (time.tm_hour == 17 && time.tm_min == 0 && time.tm_sec == 0)
        {
            g_irrigacao17hr = 1;
            g_bomba_ligada = true;
            gpio_set_level(VS, 1);
            g_tempo_inicio_bomba = esp_timer_get_time();
        }

        if (gpio_get_level(BT_REGA) == 0)
        {
            vTaskDelay(pdMS_TO_TICKS(50));
            if ((gpio_get_level(BT_REGA) == 0) && (g_bomba_ligada == false))
            {
                g_bomba_ligada = true;
                gpio_set_level(VS, 1);
                g_tempo_inicio_bomba = esp_timer_get_time();
            }
            
        }
        
        if (g_bomba_ligada)
        {
            int64_t tempo_atual = esp_timer_get_time();

            if ((tempo_atual - g_tempo_inicio_bomba) >= 60000000LL)
            {
                gpio_set_level(VS, 0);
                g_bomba_ligada = false;
            }
        }

        if (time.tm_hour == 23 && time.tm_min == 59 && time.tm_sec == 59)
        {
            g_irrigacao08hr = 0;
            g_irrigacao17hr = 0;
        }

        if (g_bomba_ligada == true)
        {
            ssd1306_display_text(g_oled, 7, "|  Irrigando!  |", false);
            ssd1306_set_line(g_oled, 0, 51, 61, 51, false);
            ssd1306_set_line(g_oled, 65, 51, 127, 51, false);
            ssd1306_display_pages(g_oled);
        } else if (g_irrigacao08hr == 1 && g_irrigacao17hr == 0) 
        {
            ssd1306_display_text(g_oled, 7, "|-08hr-|| 17hr |", false);
            ssd1306_set_line(g_oled, 0, 51, 61, 51, false);
            ssd1306_set_line(g_oled, 65, 51, 127, 51, false);
            ssd1306_display_pages(g_oled);
        } else if (g_irrigacao17hr == 1 && g_irrigacao08hr == 0) 
        {
            ssd1306_display_text(g_oled, 7, "| 08hr ||-17hr-|", false);
            ssd1306_set_line(g_oled, 0, 51, 61, 51, false);
            ssd1306_set_line(g_oled, 65, 51, 127, 51, false);
            ssd1306_display_pages(g_oled);
        } else if (g_irrigacao17hr == 1 && g_irrigacao08hr == 1) 
        {
            ssd1306_display_text(g_oled, 7, "|-08hr-||-17hr-|", false);
            ssd1306_set_line(g_oled, 0, 51, 61, 51, false);
            ssd1306_set_line(g_oled, 65, 51, 127, 51, false);
            ssd1306_display_pages(g_oled);
        } else
        {
            ssd1306_display_text(g_oled, 7, "| 08hr || 17hr |", false);
            ssd1306_set_line(g_oled, 0, 51, 61, 51, false);
            ssd1306_set_line(g_oled, 65, 51, 127, 51, false);
            ssd1306_display_pages(g_oled);
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}