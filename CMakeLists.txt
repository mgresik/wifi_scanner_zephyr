cmake_minimum_required(VERSION 3.20.0)
set (CMAKE_CXX_STANDARD 20)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(esp-zephyr)

# add_subdirectory(src)

set(APP_SOURCES
    src/main.cpp
    src/scanner.cpp
    src/oled.cpp
    src/soil_sensor.cpp
)

target_sources(app PRIVATE ${APP_SOURCES})