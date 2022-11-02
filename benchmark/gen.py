# этот скрипт создает ряд файлов со случайными длинными числами
# для тестирования скорости работы алгоритмов умножения

from random import randint
from gmpy2 import mpz  # для работы с большими числами
from tqdm import tqdm, trange
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
    size = sum(powers) + sum([a + b for a in powers for b in powers])
    return sizeof_fmt(size)


print('You need', get_folder_size(START_POWER, END_POWER),
      'of free space to generate test data')
i = input('Continue? (y/n) ')
if i != 'y':
    exit()


def generate_num(digits):
    # генерирует случайное число длины digits, причем первая цифра не может быть 0
    return ''.join([
        str(randint(0 if i else 1, 9))
        for i in range(digits)
    ])


# создаем папку, если её нет
if not os.path.exists(folder):
    os.mkdir(folder)

# создаем файлы
for power in trange(START_POWER, END_POWER + 1, desc='Generating numbers'):
    digits = 2 ** power
    number = generate_num(digits)

    with open(os.path.join(folder, f'{power}.txt'), 'w') as f:
        f.write(number)
        time.sleep(0.1)

# создаем файлы с ответами
for pow_1 in trange(START_POWER, END_POWER + 1,                  desc='Multiplying number 1'):
    for pow_2 in trange(START_POWER, END_POWER + 1, leave=False, desc='         by number 2'):
        # считываем числа из файлов
        with open(os.path.join(folder, f'{pow_1}.txt'), 'r') as f:
            number_1 = f.read()

        with open(os.path.join(folder, f'{pow_2}.txt'), 'r') as f:
            number_2 = f.read()

        with open(os.path.join(folder, f'{pow_1}_{pow_2}.txt'), 'w') as f:
            ans = mpz(number_1) * mpz(number_2)
            ans_str = ans.digits()
            f.write(ans_str)

        time.sleep(0.1)
