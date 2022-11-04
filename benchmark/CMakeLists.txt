cmake_minimum_required(VERSION 3.14)

# Название проекта
project(benchmark)

# fix build with export LANG=C export LC_ALL=C
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

FetchContent_Declare(tabulate
    GIT_REPOSITORY https://github.com/p-ranav/tabulate
    # GIT_TAG v3.11.2
)

# Добавляем библиотеку
FetchContent_MakeAvailable(tabulate)

# Создаем исполняемый файл
add_executable(benchmark benchmark.cpp)

# Связываем исполняемый файл с библиотеками
target_link_libraries(benchmark PUBLIC number fourier tabulate)