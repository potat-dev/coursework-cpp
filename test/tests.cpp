#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#define CATCH_CONFIG_MAIN

#include "../src/number/number.h"

using namespace std;

TEST_CASE("Numbers Methods", "[number]") {
  SECTION("Constructor") {
    Number a("123456789012345678901234567890");
    Number b("123456789012345678901234567890");

    REQUIRE(a.to_string() == "123456789012345678901234567890");
    REQUIRE(b.to_string() == "123456789012345678901234567890");
  }

  SECTION("Multiplication") {
    Number a("123456789012345678901234567890");
    Number b("123456789012345678901234567890");
    Number c = fft_multiply(a, b);

    REQUIRE(c.to_string() ==
            "15241578753238836750495351562536198787501905199875019052100");
  }
}

TEST_CASE("Numbers Benchmark", "[benchmark]") {
  Number a("123456789012345678901234567890");
  Number b("123456789012345678901234567890");

  BENCHMARK("FFT Multiplication") { return fft_multiply(a, b); };

  BENCHMARK("Naive Multiplication") { return column_multiply(a, b); };

  // BENCHMARK("Karatsuba Multiplication") { return karatsuba_multiply(a, b); };
}