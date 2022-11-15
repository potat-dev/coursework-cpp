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

### Примеры использования

<details>
  <summary>
    Умножение двух чисел из файлов
  </summary>
  <p></p>

```console
> multiplier 1.txt 2.txt out.txt
Multiplying... (press Ctrl + C to interrupt)
```

Более подробный вывод:

```console
> multiplier 1.txt 2.txt out.txt -v
Number 1 size: 42
Number 2 size: 4200

Multiplying... (press Ctrl + C to interrupt)

Result size: 4242
Result saved to out.txt
```

Использование бенчмарка:

```console
> multiplier 1.txt 2.txt out.txt -b 100
Multiplying... (press Ctrl + C to interrupt)

Average time: 0.0042 ms / iter
```

</details>

<details>
  <summary>
    Интерактивный режим
  </summary>
  <p></p>

```console
> multiplier -i
Enter two big numbers to multiply them
Press Ctrl + C or leave input empty to exit

Enter first number:
[1] > -123456789

Enter second number:
[1] > 123456789123456789

Result:
[1] > -15241578765432099750190521

Enter first number:
[2] > aboba
[x] Invalid number

Enter first number:
[3] >

Bye! Have a nice day :)
```

Более подробный вывод:

```console
> multiplier -i -v
Enter two big numbers to multiply them
Press Ctrl + C or leave input empty to exit

Enter first number:
[1] > 11111111111111111111
[i] Digits count: 20

Enter second number:
[1] > 123456789
[i] Digits count: 9

Result:
[1] > 1371742099999999999986282579
[i] Digits count: 28
```

Использование бенчмарка:

```console
> multiplier -i -v -b 100
Enter two big numbers to multiply them
Press Ctrl + C or leave input empty to exit

[i] Each operation will be repeated 100 times

Enter first number:
[1] > 111111111111111111111111111111
[i] Digits count: 30

Enter second number:
[1] > 1234567890
[i] Digits count: 10

Result:
[1] > 137174209999999999999999999999862825790
[i] Digits count: 39

[i] Average time: 0.00445 ms / iter
```

</details>

## Тестирование

```console
ctest
```
