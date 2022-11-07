#pragma once

#include <string>
#include <tabulate/table.hpp>
#include <vector>

// float to string with auto precision
template <typename T>
std::string to_str(T f) {
  std::stringstream ss;
  ss << f;
  return ss.str();
}

// template function to print table
template <typename T>
void ptable(std::vector<std::vector<T>> table, int axis_offset = 0,
            int split = 10) {
  int rows = table.size();
  int splitted_parts = rows / split + (rows % split != 0);
  for (int table_n = 0; table_n < splitted_parts; table_n++) {
    tabulate::Table t;
    t.format().font_align(tabulate::FontAlign::right).locale("C");  // to export LANG=C
    // add header
    int columns = std::min(split, (int)rows - table_n * split);
    tabulate::Table::Row_t header;
    header.push_back("i");
    for (int i = 0; i < columns; i++) {
      header.push_back(to_str(table_n * split + i + axis_offset));
    }
    t.add_row(header);
    // add rows with data
    for (int row = 0; row < rows; row++) {
      tabulate::Table::Row_t r;
      r.push_back(to_str(row + axis_offset));
      for (int column = 0; column < columns; column++) {
        int index = table_n * split + column;
        r.push_back(to_str(table[row][index]));
      }
      t.add_row(r);
    }
    t[0].format().font_style({tabulate::FontStyle::bold}).font_color(tabulate::Color::cyan);
    t.column(0).format().font_style({tabulate::FontStyle::bold}).font_color(tabulate::Color::cyan);
    std::cout << t << std::endl << std::endl;
  }
}

void load_cache(std::string filename, std::vector<std::vector<int>> *iters,
                std::vector<std::vector<long double>> *times, int *total_time,
                int min_power, int max_power, int min_iter, int min_seconds,
                int max_seconds, int at_least_one_iteration);

void save_cache(std::string filename, std::vector<std::vector<int>> &iters,
                std::vector<std::vector<long double>> &times, int total_time,
                int min_power, int max_power, int min_iter, int min_seconds,
                int max_seconds, int at_least_one_iteration);
