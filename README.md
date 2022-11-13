# Сoursework С++

Курсовая работа по дисциплине «Основы программирования»

## Тема работы

```plaintext
Умножение длинных чисел используя быстрое преобразование Фурье
```

Задания курсовой работы:

- Реализовать алгоритм быстрого преобразования Фурье
- На его основе релизовать алгоритм умножения длинных чисел
- Программа должна уметь считывать числа из файлов
- Программа должна иметь интерфейс командной строки

Прочитать описание курсовой работы и используемых алгоримов можно [здесь](https://potat.me/course-cpp)

## Сборка

```console
mkdir build && cd build
cmake ..
cmake --build .
```

При сборке проекта на Linux вместо `cmake --build .` можно использовать `make`

[![CMake](https://github.com/cyberpotat42/coursework-cpp/actions/workflows/cmake.yml/badge.svg)](https://github.com/cyberpotat42/coursework-cpp/actions/workflows/cmake.yml)

## Запуск

```console
multiplier [OPTIONS] [file 1] [file 2] [output]
```

### Параметры

- `-h, --help` - вывод справки
- `-v, --verbose` - вывод дополнительной информации
- `-i, --interactive` - интерактивный режим
- `-c, --column` - использовать умножение столбиком
- `-b, --benchmark` - выполнить бенчмарк (указывается количество итераций)

## Тестирование

```console
ctest
```
