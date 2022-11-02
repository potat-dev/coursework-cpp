#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "../src/number/number.h"

#define MIN_POWER 0
#define MAX_POWER 24

#define MAX_COLUMNS 10

// #define MIN_SECONDS 1
// #define MAX_SECONDS 10

constexpr int POWERS_TOTAL = MAX_POWER - MIN_POWER + 1;

#define FOLDER "../../benchmark/numbers/"

using namespace std;

double measure_time(int N, function<void()> func) {
  auto start = chrono::high_resolution_clock::now();
  for (int i = 0; i < N; i++) func();
  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
  return (double)duration.count() / N / 1000;  // return in milliseconds
}

void print_table(vector<vector<double>> table, int axis_offset, int split) {
  int rows = table.size();
  int splitted_parts = rows / split + (rows % split != 0);
  for (int table_n = 0; table_n < splitted_parts; table_n++) {
    int columns = min(split, (int)rows - table_n * split);
    for (int i = 0; i < columns; i++) {
      cout << '\t' << i + axis_offset + table_n * split;
    }
    cout << endl;
    for (int row = 0; row < rows; row++) {
      cout << row + axis_offset << "\t";
      for (int column = 0; column < columns; column++) {
        int index = table_n * split + column;
        cout << table[row][index] << '\t';
      }
      cout << endl;
    }
    cout << endl;
  }
}

int main(int argc, char **argv) {
  bool use_column = false;

  Number (*mult)(const Number &, const Number &) =
      use_column ? column_multiply : fft_multiply;

  vector<Number> numbers(POWERS_TOTAL);
  Number result;

  vector<vector<double>> times(POWERS_TOTAL, vector<double>(POWERS_TOTAL));

  for (int i = MIN_POWER; i <= MAX_POWER; i++) {
    string filename = FOLDER + to_string(i) + ".txt";
    numbers[i - MIN_POWER].load(filename);
  }

  for (int a = 0; a < POWERS_TOTAL; a++) {
    for (int b = 0; b < POWERS_TOTAL; b++) {
      auto func = [&]() { result = mult(numbers[a], numbers[b]); };
      times[a][b] = measure_time(1, func);
    }
  }

  print_table(times, MIN_POWER, MAX_COLUMNS);
}