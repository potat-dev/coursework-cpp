#include <CLI/CLI.hpp>
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "../src/number/number.h"
#include "utils.hpp"

using namespace std;

// Test numbers folder (created by gen.py)
#define FOLDER "../../benchmark/numbers/"

// Start and end powers of 2 for the benchmark
// #define MIN_POWER 0
// #define MAX_POWER 18

// Total powers (table size)
// constexpr int POWERS_TOTAL = MAX_POWER - MIN_POWER + 1;

// Iterations per pair of numbers
// #define MIN_ITERATIONS 5
// #define MIN_SECONDS 2
// #define MAX_SECONDS 10

// MAX_SECONDS has more priority than MIN_ITERATIONS
// (to avoid too long operations with uneficient algorithms)
// Too long operations will be cutted to MAX_SECONDS or canceled

// Flag to override this (cancel) behavior
// If defined, then minimum number of iterations is its value
// #define AT_LEAST_ITERATIONS 1

// If this flag is defined, then iters will be loaded from cache
// (to avoid long startup time before the benchmark)
// If this flag is not defined, then iters will be recalculated and cached
// #define LOAD_FROM_CACHE
// #define CACHE_FILE "cache.txt"

// Export results to CSV file
// If this flag is defined, then results will be exported to CSV file
// #define CSV_FILE "results.csv"

// Print debug information and tables
// #define DEBUG

// Table max columns (for wrapping)
// #define MAX_COLUMNS 8

#define EXEC "benchmark.exe"
#define TITLE "\nBig Number Multiplier Benchmark - v1.0"
#define FOOTER \
  "Created with <3 by Cyber Potato (Denis Churilov) at SUAI University"

int main(int argc, char **argv) {
  // create CLI app
  CLI::App app(TITLE, EXEC);
  app.footer(FOOTER);

  Settings config;
  int power_1 = 0, power_2 = -1;

  app.add_option("Power 1", power_1,
                 "Min power of 2; Powers count if power 2 is n/d")
      ->option_text("(uint)")
      ->required();
  app.add_option("Power 2", power_2, "Max power of 2")->option_text("(uint)");

  // parse CLI arguments
  CLI11_PARSE(app, argc, argv);

  if (power_1 < 0 || power_2 < -1) {
    cout << "Powers must be positive" << endl;
    return 1;
  }
  if (power_2 == -1) {
    if (power_1 == 0) {
      cout << "Zero powers count" << endl;
      return 1;
    }
    power_2 = power_1 - 1;
    power_1 = 0;
  }
  if (power_1 > power_2) {
    cout << "Power 1 must be less than power 2" << endl;
    return 1;
  }

  config.min_power = power_1;
  config.max_power = power_2;
  config.powers_total = power_2 - power_1 + 1;

  cout << power_1 << " " << power_2 << endl;
}

int benchmark(Settings &config) {
  Number (*mult)(const Number &, const Number &) =
      config.use_column ? column_multiply : fft_multiply;

  vector<Number> numbers(config.powers_total);
  Number result;

  vector<vector<long double>> times(config.powers_total,
                                    vector<long double>(config.powers_total));
  vector<vector<int>> iters(config.powers_total,
                            vector<int>(config.powers_total));
  long double total_time = 0;

  if (config.verbose) cout << "Loading numbers... ";

  for (int i = config.min_power; i <= config.max_power; i++) {
    string filename = FOLDER + to_string(i) + ".txt";
    numbers[i - config.min_power].load(filename);
  }

  if (config.verbose) cout << "Done!" << endl;

  if (!config.load_cache) {
    cout << "Measuring speed for more efficient benchmarking...\n" << endl;

    for (int a = 0; a < config.powers_total; a++) {
      cout << "Measuring Len 2^" << a + config.min_power << "... ";
      auto loop_start = chrono::high_resolution_clock::now();
      for (int b = a; b < config.powers_total; b++) {
        auto start = chrono::high_resolution_clock::now();
        result = mult(numbers[a], numbers[b]);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
        times[a][b] = (long double)duration.count() /
                      1000000.0;  // return in milliseconds
      }
      auto loop_end = chrono::high_resolution_clock::now();
      auto loop_duration =
          chrono::duration_cast<chrono::milliseconds>(loop_end - loop_start);
      cout << "Done! (" << loop_duration.count() << " ms)" << endl;
    }
    cout << endl;

    // calculate iters for each pair to run between config.min_seconds and
    // config.max_seconds
    for (int a = 0; a < config.powers_total; a++) {
      for (int b = a; b < config.powers_total; b++) {
        // calculate min and max iterations
        int iters_for_min_seconds =
            (int)round(config.min_seconds * 1000 / times[a][b]);
        int iters_for_max_seconds =
            (int)round(config.max_seconds * 1000 / times[a][b]);
        // calculate iterations count
        iters[a][b] = max(config.min_iters, iters_for_min_seconds);
        iters[a][b] = min(iters[a][b], iters_for_max_seconds);
        if (config.at_least_iters > 0)
          iters[a][b] = max(iters[a][b], config.at_least_iters);
        // calculate total time in milliseconds
        total_time += times[a][b] * iters[a][b];
      }
    }

    // save iters and times to txt file
    // (to avoid long calculations in the future)
    // save settings too (to check if they are the same)
    save_cache(config.cache_file, iters, times, total_time, config.min_power,
               config.max_power, config.min_iters, config.min_seconds,
               config.max_seconds, config.at_least_iters);

  } else {  // LOAD_FROM_CACHE
    // check settings stored in txt file, load times and iters
    load_cache(config.cache_file, &iters, &times, &total_time, config.min_power,
               config.max_power, config.min_iters, config.min_seconds,
               config.max_seconds, config.at_least_iters);
  }

  if (config.verbose) {
    cout << "Time for 1 iteration of each pair (ms):\n" << endl;
    ptable(times, config.min_power, config.max_columns);

    cout << "Iterations for each pair:\n" << endl;
    ptable(iters, config.min_power, config.max_columns);
  }

  cout << "Time to run all iterations: " << total_time / 1000 << " seconds";
  cout << " (" << total_time / 1000 / 60 << " minutes)\n" << endl;

  // run all iterations
  for (int a = 0; a < config.powers_total; a++) {
    for (int b = a; b < config.powers_total; b++) {
      auto start = chrono::high_resolution_clock::now();
      for (int i = 0; i < iters[a][b]; i++)
        result = mult(numbers[a], numbers[b]);
      auto end = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
      times[a][b] = (long double)duration.count() / 1000000.0 / iters[a][b];
      times[b][a] = times[a][b];
      if (config.verbose) {
        cout << "Len 2^" << a + config.min_power << " * Len 2^"
             << b + config.min_power;
        cout << ":\t" << times[a][b] << " ms" << endl;
      }
    }
  }

  if (config.verbose) {
    cout << endl << "Average time for each pair (ms):\n" << endl;
    ptable(times, config.min_power, config.max_columns);
  }

  if (config.csv_file != "") {
    // export results to CSV file
    cout << "Exporting results to CSV file " << config.csv_file << "... ";
    export_csv(times, config.csv_file, config.min_power);
    cout << "Done!" << endl;
  }

  return 0;
}