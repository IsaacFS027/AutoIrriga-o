#include "hardware_init.h"
#include "variaveis.h"
#include "dht.h"

// Alocação real da memória dos handles
i2c_master_bus_handle_t g_i2c_bus = NULL;
ssd1306_handle_t g_oled = NULL;
ds1302_t g_rtc_dev;

esp_err_t hardware_init(void)
{
    esp_err_t ret = ESP_OK;

    // 1. Configuração do barramento I2C
    i2c_master_bus_config_t i2c_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = I2C_SDA,
        .scl_io_num = I2C_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ret = i2c_new_master_bus(&i2c_config, &g_i2c_bus);
    if (ret != ESP_OK) return ret;

    // 2. Configuração do Display OLED
    ssd1306_config_t oled_config = I2C_SSD1306_128x64_CONFIG_DEFAULT;
    ret = ssd1306_init(g_i2c_bus, &oled_config, &g_oled);
    if (ret != ESP_OK) return ret;

    // 3. Limpa a tela
    ret = ssd1306_clear_display(g_oled, false);
    if (ret != ESP_OK) return ret;

    // 4. Configuração e inicialização do RTC DS1302
    g_rtc_dev.ce_pin   = DS1302_CE;
    g_rtc_dev.io_pin   = DS1302_IO;
    g_rtc_dev.sclk_pin = DS1302_CLK;

    ret = ds1302_init(&g_rtc_dev);
    if (ret != ESP_OK) return ret;

    ds1302_start(&g_rtc_dev, true);

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BT_REGA),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,   // Habilita resistor de pull-up interno
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    gpio_reset_pin(GPIO_NUM_15);
    gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_15, 0);

    return ESP_OK;
}

esp_err_t dht_ler_dados(void)
{
    int16_t temp = 0;
    int16_t umid = 0;

    // O driver lê os valores em décimos (ex: 250 = 25.0°C)
    esp_err_t res = dht_read_data(DHT_TYPE_DHT11, DHT_GPIO_PIN, &umid, &temp);

    if (res == ESP_OK) {
        g_temperatura = temp / 10.0f;
        g_humidade = umid / 10;
    }

    return res;
}