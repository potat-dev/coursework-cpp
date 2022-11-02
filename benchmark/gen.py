# этот скрипт создает ряд файлов со случайными длинными числами
# для тестирования скорости работы алгоритмов умножения

from random import randint
from gmpy2 import mpz  # для работы с большими числами
from tqdm import trange
import os
import time

# длина чисел: от 2 ^ START_POWER до 2 ^ END_POWER
START_POWER = 0
END_POWER = 24

# папка, в которой будут созданы файлы
folder = 'benchmark/numbers'


def get_folder_size(start_power, end_power):
    # возвращает размер создаваемой папки в байтах
    def sizeof_fmt(num, suffix="B"):
        # возвращает размер в удобном формате
        for unit in ["", "Ki", "Mi", "Gi", "Ti", "Pi", "Ei", "Zi"]:
            if abs(num) < 1024.0:
                return f"{num:3.1f} {unit}{suffix}"
            num /= 1024.0
        return f"{num:.1f} Yi{suffix}"

    powers = [2**i for i in range(start_power, end_power + 1)]
    matrix = [
        2**pow_1 + 2**pow_2
        for pow_1 in range(start_power, end_power + 1)
        for pow_2 in range(pow_1, end_power + 1)
    ]
    size = sum(powers) + sum(matrix)
    return sizeof_fmt(size)


def generate_num(digits):
    # генерирует случайное число длины digits, причем первая цифра не может быть 0
    return ''.join([
        str(randint(0 if i else 1, 9))
        for i in range(digits)
    ])


print('You need', get_folder_size(START_POWER, END_POWER),
      'of free space to generate test data')

if input('Continue? (y/n) ') != 'y':
    exit()

# создаем папку, если её нет
if not os.path.exists(folder):
    os.mkdir(folder)

# создаем случайные числа и записываем их в файлы
numbers = []
for power in trange(START_POWER, END_POWER + 1, desc='Generating numbers'):
    digits = 2 ** power
    number = generate_num(digits)
    numbers.append(mpz(number))
    with open(os.path.join(folder, f'{power}.txt'), 'w') as f:
        f.write(number)
        time.sleep(0.1)

# создаем файлы с ответами
for pow_1 in trange(START_POWER, END_POWER + 1,            desc='Multiplying number'):
    for pow_2 in trange(pow_1, END_POWER + 1, leave=False, desc='  by second number'):
        with open(os.path.join(folder, f'{pow_1}_{pow_2}.txt'), 'w') as f:
            ans = numbers[pow_1] * numbers[pow_2]
            ans_str = ans.digits()
            f.write(ans_str)
        time.sleep(0.1)
