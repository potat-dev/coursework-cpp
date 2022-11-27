cmake_minimum_required(VERSION 3.14)

# cxx standard
set (CMAKE_CXX_STANDARD 11)

# Название проекта
project(coursework)

# Расположение заголовочных файлов
include_directories(modes)
include_directories(number)
include_directories(fourier)

# Поиск библиотеки CLI11
include(FetchContent)
FetchContent_Declare(
  cli11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11
  GIT_TAG        v2.2.0
)

FetchContent_MakeAvailable(cli11)

# Добавление подпроектов, указывается имя дирректории
add_subdirectory(modes)
add_subdirectory(number)
add_subdirectory(fourier)