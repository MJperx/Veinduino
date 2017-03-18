cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME Veinduino)
project(${PROJECT_NAME})

set(ARDUINO_DEFAULT_PORT /dev/cu.usbmodemfd121)

set(${CMAKE_PROJECT_NAME}_SKETCH Veinduino.ino)
