cmake_minimum_required(VERSION 3.14)

# Название проекта
project(benchmark)

FetchContent_Declare(tabulate
    GIT_REPOSITORY https://github.com/p-ranav/tabulate
)

# Добавляем библиотеку
FetchContent_MakeAvailable(tabulate)

# Создаем исполняемый файл
add_executable(benchmark benchmark.cpp utils.cpp)

# Связываем исполняемый файл с библиотеками
target_link_libraries(benchmark PUBLIC number fourier tabulate)