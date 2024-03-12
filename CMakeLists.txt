cmake_minimum_required(VERSION 3.22)
project(SOURCE)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(Source
        ADXL343.c
        ADXL343.h
        CommandHandle.c
        CommandHandle.h
        FunctionStatus.h
        i2c_driver.c
        i2c_driver.h
        main.c)
