cmake_minimum_required(VERSION 3.14)

# cxx standard
set (CMAKE_CXX_STANDARD 11)

# название проекта
project(coursework)

# тестирование
enable_testing()

# добавление подпроектов
add_subdirectory(src)
add_subdirectory(tests)

# Создает исполняемый файл с именем coursework
add_executable(multiplier src/main.cpp)

# Линковка программы с библиотекой
target_link_libraries(multiplier modes number fourier CLI11::CLI11)

# Опции компиляции
target_compile_options(multiplier PRIVATE -Wall)