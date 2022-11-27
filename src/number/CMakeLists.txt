cmake_minimum_required(VERSION 3.14)

# cxx standard
set (CMAKE_CXX_STANDARD 11)

# Название проекта
project(number)

# Установка переменной со списком исходников
set(SOURCE_LIB number.cpp)

# Создание статической библиотеки
add_library(number STATIC ${SOURCE_LIB})