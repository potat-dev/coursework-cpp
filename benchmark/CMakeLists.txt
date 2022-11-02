cmake_minimum_required(VERSION 3.14)

# Название проекта
project(benchmark)

# include_directories(../src/number)

# search for static library number
# find_library(NUMBER_LIB number ../src/number)

# include target number



# add library ../src/number
# add_subdirectory(../src/number number)

# add library ../src/number
# add_subdirectory(../src/number number)

# Создаем исполняемый файл
add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark PUBLIC number fourier)