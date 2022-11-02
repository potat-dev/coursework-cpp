#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "../src/number/number.h"

using namespace std;

// Test numbers folder (created by gen.py)
#define FOLDER "../../benchmark/numbers/"

// Start and end powers of 2 for the benchmark
#define MIN_POWER 13
#define MAX_POWER 15

// Total powers (table size)
constexpr int POWERS_TOTAL = MAX_POWER - MIN_POWER + 1;

// Iterations per pair of numbers
#define MIN_ITERATIONS 5
#define MIN_SECONDS 2
#define MAX_SECONDS 10

// MAX_SECONDS has more priority than MIN_ITERATIONS
// (to avoid too long operations with uneficient algorithms)
// Too long operations will be cutted to MAX_SECONDS or canceled

// Flag to override this (cancel) behavior
// If this flag is defined, then minimum number of iterations is 1
#define AT_LEAST_ONE_ITERATION

// Print debug information and tables
#define DEBUG

// Table max columns (for wrapping)
#define MAX_COLUMNS 10

template <typename T>
// template function to print table
void ptable(vector<vector<T>> table, int axis_offset = 0, int split = 10,
            int tab_size = 8, int index_width = 3, bool round_val = false) {
  int rows = table.size();
  int splitted_parts = rows / split + (rows % split != 0);
  for (int table_n = 0; table_n < splitted_parts; table_n++) {
    int columns = min(split, (int)rows - table_n * split);
    cout << setw(index_width + 3) << " | ";
    for (int i = 0; i < columns; i++) {
      cout << setw(tab_size) << i + axis_offset + table_n * split;
    }
    cout << endl;
    cout << string(index_width, '-') << "-+-";
    cout << string(columns * tab_size, '-') << endl;

    for (int row = 0; row < rows; row++) {
      cout << setw(index_width) << row + axis_offset << " | ";
      for (int column = 0; column < columns; column++) {
        int index = table_n * split + column;
        cout << setw(tab_size);
        cout << (round_val ? round(table[row][index]) : table[row][index]);
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
  vector<vector<int>> iters(POWERS_TOTAL, vector<int>(POWERS_TOTAL));

#ifdef DEBUG
  cout << "Loading numbers... ";
#endif

  for (int i = MIN_POWER; i <= MAX_POWER; i++) {
    string filename = FOLDER + to_string(i) + ".txt";
    numbers[i - MIN_POWER].load(filename);
  }

#ifdef DEBUG
  cout << "Done!" << endl;
  cout << "Measuring speed for more efficient benchmarking..." << endl;
#endif

  for (int a = 0; a < POWERS_TOTAL; a++) {
    for (int b = 0; b < POWERS_TOTAL; b++) {
      // #ifdef DEBUG // affects performance
      //       cout << "Measuring 2^" << a + MIN_POWER;
      //       cout << " digits * 2^" << b + MIN_POWER << " digits..." << endl;
      // #endif
      auto start = chrono::high_resolution_clock::now();
      result = mult(numbers[a], numbers[b]);
      auto end = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
      times[a][b] = (double)duration.count() / 1000;  // return in milliseconds
    }
  }

  // total time for all iterations of all pairs
  long double total_time = 0;

  // calculate iters for each pair to run between MIN_SECONDS and MAX_SECONDS
  for (int a = 0; a < POWERS_TOTAL; a++) {
    for (int b = 0; b < POWERS_TOTAL; b++) {
      // calculate min and max iterations
      int iters_for_min_seconds = (int)round(MIN_SECONDS * 1000 / times[a][b]);
      int iters_for_max_seconds = (int)round(MAX_SECONDS * 1000 / times[a][b]);
      // calculate iterations count
      iters[a][b] = max(MIN_ITERATIONS, iters_for_min_seconds);
      iters[a][b] = min(iters[a][b], iters_for_max_seconds);
#ifdef AT_LEAST_ONE_ITERATION
      iters[a][b] = max(iters[a][b], 1);
#endif
      // calculate total time in milliseconds
      total_time += times[a][b] * iters[a][b];
    }
  }

#ifdef DEBUG
  cout << "Time for 1 iteration of each pair (ms):\n" << endl;
  ptable(times, MIN_POWER, MAX_COLUMNS);

  cout << "Iterations for each pair:\n" << endl;
  ptable(iters, MIN_POWER, MAX_COLUMNS);
#endif

  cout << "Time to run all iterations: " << total_time / 1000 << " seconds";
  cout << " (" << total_time / 1000 / 60 << " minutes)\n" << endl;

  // run all iterations
  for (int a = 0; a < POWERS_TOTAL; a++) {
    for (int b = 0; b < POWERS_TOTAL; b++) {
      auto start = chrono::high_resolution_clock::now();
      for (int i = 0; i < iters[a][b]; i++)
        result = mult(numbers[a], numbers[b]);
      auto end = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
      times[a][b] = (double)duration.count() / 1000 / iters[a][b];
#ifdef DEBUG
      cout << "2^" << a + MIN_POWER << " digits * 2^" << b + MIN_POWER;
      cout << " digits: " << times[a][b] << " ms" << endl;
#endif
    }
  }

#ifdef DEBUG
  cout << endl << "Average time for each pair (ms):\n" << endl;
  ptable(times, MIN_POWER, MAX_COLUMNS, 10);
#endif

  return 0;
}