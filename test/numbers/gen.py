from random import randint
from os.path import join

import time
from gmpy2 import mpz

folder = 'test'
digits_1 = 1000
digits_2 = 1000

def generate_num(digits):
  return ''.join([str(randint(0 if i else 1, 9)) for i in range(digits)])

number_1 = generate_num(digits_1)
number_2 = generate_num(digits_2)

with open(join(folder, '1.txt'), 'w') as f:
  print("Number 1 len:", len(number_1))
  f.write(number_1)

with open(join(folder, '2.txt'), 'w') as f:
  print("Number 2 len:", len(number_2))
  f.write(number_2)

print()
with open(join(folder, 'ans.txt'), 'w') as f:
  print("Multiplying numbers...") # just to show that the program is still running
  start = time.time()
  ans = mpz(number_1) * mpz(number_2)
  end = time.time()
  print("Time taken to multiply:", end - start)
  start = time.time()
  str_ans = ans.digits()
  end = time.time()
  print("Time taken to convert to string:", end - start)
  print("Answer len:", len(str_ans))
  f.write(str_ans)
  # f.write(str(int(number_1) * int(number_2)))