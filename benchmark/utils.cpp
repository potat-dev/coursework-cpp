#include "utils.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

void load_cache(string filename, vector<vector<int>> *iters,
                vector<vector<long double>> *times, long double *total_time,
                int min_power, int max_power, int min_iter, int min_seconds,
                int max_seconds, int at_least_one_iteration) {
  // load cache
  ifstream file(filename);
  if (!file.is_open()) {
    cout << "Error: cache file not found" << endl;
    return;
  }
  // load settings
  int _min_power, _max_power, _min_iter, _min_seconds, _max_seconds;
  bool _at_least_one_iteration;
  file >> _min_power >> _max_power >> _min_iter >> _min_seconds >>
      _max_seconds >> _at_least_one_iteration >> *total_time;
  // check if settings are the same
  if (_min_power != min_power || _max_power != max_power ||
      _min_iter != min_iter || _min_seconds != min_seconds ||
      _max_seconds != max_seconds ||
      _at_least_one_iteration != at_least_one_iteration) {
    cout << "Error: cache settings are not the same" << endl;
    throw runtime_error("cache settings are not the same");
  }
  // load iters
  for (int i = 0; i < iters->size(); i++) {
    for (int j = 0; j < (*iters)[i].size(); j++) {
      file >> (*iters)[i][j];
    }
  }
  // load times
  for (int i = 0; i < times->size(); i++) {
    for (int j = 0; j < (*times)[i].size(); j++) {
      file >> (*times)[i][j];
    }
  }
  file.close();
}

void save_cache(string filename, vector<vector<int>> &iters,
                vector<vector<long double>> &times, long double total_time,
                int min_power, int max_power, int min_iter, int min_seconds,
                int max_seconds, int at_least_one_iteration) {
  // save cache
  ofstream file(filename);
  if (!file.is_open()) {
    cout << "Error: cache file not found" << endl;
    return;
  }
  // save settings
  file << min_power << " " << max_power << " " << min_iter << " " << min_seconds
       << " " << max_seconds << " " << at_least_one_iteration << " "
       << total_time << " ";
  // save iters
  for (int i = 0; i < iters.size(); i++) {
    for (int j = 0; j < iters[i].size(); j++) {
      file << iters[i][j] << " ";
    }
  }
  // save times
  for (int i = 0; i < times.size(); i++) {
    for (int j = 0; j < times[i].size(); j++) {
      file << times[i][j] << " ";
    }
  }
  file.close();
}