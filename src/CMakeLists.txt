cmake_minimum_required(VERSION 3.14)

# Название проекта
project(coursework)

# Установка переменной со списком исходников
set(SOURCE_EXE main.cpp)

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

# Создает исполняемый файл с именем main
add_executable(multiplier ${SOURCE_EXE})

# Добавление подпроектов, указывается имя дирректории
add_subdirectory(modes)
add_subdirectory(number)
add_subdirectory(fourier)

# Линковка программы с библиотекой
target_link_libraries(multiplier modes number fourier CLI11::CLI11)