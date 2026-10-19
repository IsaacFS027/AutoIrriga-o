idf_component_register(
    SRCS "main.c" "variaveis.c" "hardware_init.c"
    INCLUDE_DIRS "."
    REQUIRES ds1302 k0i05__esp_ssd1306 esp-idf-lib__dht esp_timer
)