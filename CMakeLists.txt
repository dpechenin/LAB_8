cmake_minimum_required(VERSION 3.27)
project(lab8 C)

set(CMAKE_C_STANDARD 11)

add_executable(lab8 main.c
        header.h
        flat.h
        fio.h
        func.c)
