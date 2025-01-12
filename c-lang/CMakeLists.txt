CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(uspark C)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

INCLUDE_DIRECTORIES(.)

SET(uspark_src
    uspark.c
    libowfat/str_len.c
    libowfat/str_diff.c
    libowfat/str_start.c
    libowfat/scan_double.c
)

ADD_EXECUTABLE(uspark ${uspark_src})
