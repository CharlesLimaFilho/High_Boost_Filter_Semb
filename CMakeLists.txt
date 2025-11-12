cmake_minimum_required(VERSION 3.27)
project(High_Boost_Filter_Semb C)

set(CMAKE_C_STANDARD 11)

include_directories(implementacoes_C)

add_executable(High_Boost_Filter_Semb
        implementacoes_C/high_boost.c
        implementacoes_C/high_boost.h
        implementacoes_C/main.c)
