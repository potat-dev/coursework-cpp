cmake_minimum_required(VERSION 3.14)

# cxx standard
set (CMAKE_CXX_STANDARD 11)

# Название проекта
project(fourier)

# Установка переменной со списком исходников
set(SOURCE_LIB fourier.cpp)

# Создание статической библиотеки
add_library(fourier STATIC ${SOURCE_LIB})