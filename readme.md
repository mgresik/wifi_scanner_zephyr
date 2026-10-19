## Build:
Выполнять из /path/to/zephyrproject. Желательно перед работой запускать venv в этой директории
-- west build -p always -b esp32s3_devkitc/esp32s3/procpu ~/handmake/esp-zephyr
-- west flash

GPIO14 - gpio input для влажности
Выбран ADC2 по терминологии ESP, канал 3