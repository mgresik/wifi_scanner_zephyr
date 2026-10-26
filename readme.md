## Build:
Выполнять из /path/to/zephyrproject, предварительно запустив venv в этой директории
-- west build -p always -b esp32s3_devkitc/esp32s3/procpu ~/handmake/esp-zephyr -- -DDTC_OVERLAY_FILE=../../handmake/esp-zephyr/boards/esp32s3_devkit/esp32s3_devkitc_esp32s3_procpu.overlay
-- west flash

Для проверки можно вызывать:
-- west espressif monitor

GPIO14 - gpio input для влажности
Выбран ADC2 по терминологии ESP, канал 3