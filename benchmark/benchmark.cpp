#include <chrono>
#include <functional>
#include <iostream>
#include <string>

#include "../src/number/number.h"

#define MIN_POWER 0
#define MAX_POWER 24

#define FOLDER "D:\\Projects\\coursework-cpp\\benchmark\\numbers\\"

using namespace std;

double measure_time(int N, function<void()> func) {
  auto start = chrono::high_resolution_clock::now();
  for (int i = 0; i < N; i++) func();
  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
  return (double)duration.count() / N / 1000;  // return in milliseconds
}

int main(int argc, char** argv) {
  for (int i = MIN_POWER; i <= MAX_POWER; i++) {
    string filename = FOLDER + to_string(i) + ".txt";
    Number n1, n2, res;
    n1.load(filename);
    n2.load(filename);
    double time = measure_time(5, [&]() { res = fft_multiply(n1, n2); });
    cout << i << " " << time << endl;
  }
}