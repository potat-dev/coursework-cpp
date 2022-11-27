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

[1] < -123456789
[1] < 123456789123456789
[1] > -15241578765432099750190521

[2] < aboba
[x] Invalid number

[3] < (pressing enter)

Bye! Have a nice day :)
```

Более подробный вывод:

```console
> multiplier -i -v
Enter two big numbers to multiply them
Press Ctrl + C or leave input empty to exit

Enter first number:
[1] < 101010101010101010    
[i] Digits count: 18

Enter second number:
[1] < 424242424242424242     
[i] Digits count: 18

Result:
[1] > 42852770125497398138965411692684420
[i] Digits count: 35

Enter first number:
[2] < -00001
[i] Digits count: 1

Enter second number:
[2] < -0
[x] Invalid number
```

Использование бенчмарка:

```console
> multiplier -i -v -b 100
Enter two big numbers to multiply them
Press Ctrl + C or leave input empty to exit

[i] Each operation will be repeated 100 times

Enter first number:
[1] < 011010000110100100100001
[i] Digits count: 23

Enter second number:
[1] < 424242424242424242424242 
[i] Digits count: 24

Result:
[1] > 4670909137618224284848904419999953290866624242
[i] Digits count: 46

[i] Average time: 0.0042 ms / iter
```

</details>

## Тестирование

```console
ctest
```
