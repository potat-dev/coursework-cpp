#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tabulate/table.hpp>
#include <vector>

#include "../src/number/number.h"
#include "cache.hpp"

using namespace tabulate;
using namespace std;

// Test numbers folder (created by gen.py)
#define FOLDER "../../benchmark/numbers/"

// Start and end powers of 2 for the benchmark
#define MIN_POWER 0
#define MAX_POWER 5

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
// If defined, then minimum number of iterations is its value
#define AT_LEAST_ITERATIONS 1

// If this flag is defined, then iters will be loaded from cache
// (to avoid long startup time before the benchmark)
// If this flag is not defined, then iters will be recalculated and cached
// #define LOAD_FROM_CACHE
#define CACHE_FILE "cache.txt"

// TODO: Export results to CSV file
#define EXPORT_TO_CSV
#define CSV_FILE "results.csv"

// Print debug information and tables
#define DEBUG

// Table max columns (for wrapping)
#define MAX_COLUMNS 10

// float to string with auto precision
template <typename T>
std::string to_str(T f) {
  std::stringstream ss;
  ss << f;
  return ss.str();
}

// template function to print table
template <typename T>
void ptable(vector<vector<T>> table, int axis_offset = 0, int split = 10) {
  int rows = table.size();
  int splitted_parts = rows / split + (rows % split != 0);
  for (int table_n = 0; table_n < splitted_parts; table_n++) {
    Table t;
    t.format().font_align(FontAlign::right).locale("C");  // to export LANG=C
    // add header
    int columns = min(split, (int)rows - table_n * split);
    Table::Row_t header;
    header.push_back("i");
    for (int i = 0; i < columns; i++) {
      header.push_back(to_str(table_n * split + i + axis_offset));
    }
    t.add_row(header);
    // add rows with data
    for (int row = 0; row < rows; row++) {
      Table::Row_t r;
      r.push_back(to_str(row + axis_offset));
      for (int column = 0; column < columns; column++) {
        int index = table_n * split + column;
        r.push_back(to_str(table[row][index]));
      }
      t.add_row(r);
    }
    t[0].format().font_style({FontStyle::bold}).font_color(Color::cyan);
    t.column(0).format().font_style({FontStyle::bold}).font_color(Color::cyan);
    cout << t << endl << endl;
  }
}

int main(int argc, char **argv) {
  bool use_column = 1;

  Number (*mult)(const Number &, const Number &) =
      use_column ? column_multiply : fft_multiply;

  vector<Number> numbers(POWERS_TOTAL);
  Number result;

  vector<vector<long double>> times(POWERS_TOTAL,
                                    vector<long double>(POWERS_TOTAL));
  vector<vector<int>> iters(POWERS_TOTAL, vector<int>(POWERS_TOTAL));
  long double total_time = 0;

#ifdef DEBUG
  cout << "Loading numbers... ";
#endif

  for (int i = MIN_POWER; i <= MAX_POWER; i++) {
    string filename = FOLDER + to_string(i) + ".txt";
    numbers[i - MIN_POWER].load(filename);
  }

#ifdef DEBUG
  cout << "Done!" << endl;
#endif

#ifndef LOAD_FROM_CACHE
  cout << "Measuring speed for more efficient benchmarking...\n" << endl;

  for (int a = 0; a < POWERS_TOTAL; a++) {
    for (int b = a; b < POWERS_TOTAL; b++) {
      auto start = chrono::high_resolution_clock::now();
      result = mult(numbers[a], numbers[b]);
      auto end = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
      times[a][b] =
          (long double)duration.count() / 1000000.0;  // return in milliseconds
    }
    // fill the rest of the table with NaN
    // for (int b = 0; b < a; b++) times[a][b] = NAN;
  }

  // calculate iters for each pair to run between MIN_SECONDS and MAX_SECONDS
  for (int a = 0; a < POWERS_TOTAL; a++) {
    for (int b = a; b < POWERS_TOTAL; b++) {
      // calculate min and max iterations
      int iters_for_min_seconds = (int)round(MIN_SECONDS * 1000 / times[a][b]);
      int iters_for_max_seconds = (int)round(MAX_SECONDS * 1000 / times[a][b]);
      // calculate iterations count
      iters[a][b] = max(MIN_ITERATIONS, iters_for_min_seconds);
      iters[a][b] = min(iters[a][b], iters_for_max_seconds);
#ifdef AT_LEAST_ITERATIONS
      iters[a][b] = max(iters[a][b], AT_LEAST_ITERATIONS);
#endif
      // calculate total time in milliseconds
      total_time += times[a][b] * iters[a][b];
    }
  }

  // save iters and times to txt file
  // (to avoid long calculations in the future)
  // save settings too (to check if they are the same)
  save_cache(CACHE_FILE, iters, times, total_time, MIN_POWER, MAX_POWER,
             MIN_ITERATIONS, MIN_SECONDS, MAX_SECONDS, AT_LEAST_ITERATIONS);

#else  // LOAD_FROM_CACHE
  // check settings stored in txt file, load times and iters
  load_cache(CACHE_FILE, &iters, &times, &total_time, MIN_POWER, MAX_POWER,
             MIN_ITERATIONS, MIN_SECONDS, MAX_SECONDS, AT_LEAST_ITERATIONS);
#endif

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
    for (int b = a; b < POWERS_TOTAL; b++) {
      auto start = chrono::high_resolution_clock::now();
      for (int i = 0; i < iters[a][b]; i++)
        result = mult(numbers[a], numbers[b]);
      auto end = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
      times[a][b] = (long double)duration.count() / 1000000.0 / iters[a][b];
      times[b][a] = times[a][b];
#ifdef DEBUG
      cout << "Len 2^" << a + MIN_POWER << " * Len 2^" << b + MIN_POWER;
      cout << ":\t" << times[a][b] << " ms" << endl;
#endif
    }
  }

#ifdef DEBUG
  cout << endl << "Average time for each pair (ms):\n" << endl;
  ptable(times, MIN_POWER, MAX_COLUMNS);
#endif

  return 0;
}