#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <sstream>

#define CATCH_CONFIG_MAIN

#include "../src/number/number.h"

using namespace std;

TEST_CASE("Number Constructors", "[constructors]") {
  SECTION("Default Constructor") {
    Number n;
    CHECK(n.is_negative() == false);
    CHECK(n.get_digits() == vector<int>{0});
  }

  SECTION("String Constructor") {
    Number n("123");
    CHECK(n.is_negative() == false);
    CHECK(n.get_digits() == vector<int>{3, 2, 1});
  }

  SECTION("Negative String Constructor") {
    Number n("-123");
    CHECK(n.is_negative() == true);
    CHECK(n.get_digits() == vector<int>{3, 2, 1});
  }

  SECTION("Invalid String Constructor") {
    // Non numeric string
    CHECK_THROWS_AS(Number("abc"), invalid_argument);
    CHECK_THROWS_AS(Number("abc123"), invalid_argument);
    CHECK_THROWS_AS(Number("123abc"), invalid_argument);
    CHECK_THROWS_AS(Number("a123bc"), invalid_argument);
    // Empty string
    CHECK_THROWS_AS(Number(""), invalid_argument);
    CHECK_THROWS_AS(Number(" "), invalid_argument);
    CHECK_THROWS_AS(Number("-"), invalid_argument);
    // Leading zeros (allowed)
    CHECK_NOTHROW(Number("000123"));
    CHECK_NOTHROW(Number("-000123"));
    // Negative zero
    CHECK_THROWS_AS(Number("-0"), invalid_argument);
    CHECK_THROWS_AS(Number("-000"), invalid_argument);
    // Negative sign in invalid position
    CHECK_THROWS_AS(Number("1-23"), invalid_argument);
    CHECK_THROWS_AS(Number("-1-23"), invalid_argument);
    CHECK_THROWS_AS(Number("123-"), invalid_argument);
  }

  SECTION("Int64 Constructor") {
    Number n(123);
    CHECK(n.is_negative() == false);
    CHECK(n.get_digits() == vector<int>{3, 2, 1});
  }

  SECTION("Negative Int64 Constructor") {
    Number n(-123);
    CHECK(n.is_negative() == true);
    CHECK(n.get_digits() == vector<int>{3, 2, 1});
  }

  SECTION("Vector Constructor") {
    Number n(vector<int>{3, 2, 1});
    CHECK(n.is_negative() == false);
    CHECK(n.get_digits() == vector<int>{3, 2, 1});
  }

  SECTION("Negative Vector Constructor") {
    Number n(vector<int>{3, 2, 1}, true);
    CHECK(n.is_negative() == true);
    CHECK(n.get_digits() == vector<int>{3, 2, 1});
  }

  SECTION("Invalid Vector Constructor") {
    // Empty vector
    CHECK_THROWS_AS(Number(vector<int>{}), invalid_argument);
    // Vector with leading zeros (allowed)
    CHECK_NOTHROW(Number(vector<int>{0, 0, 1, 2, 3}));
    // Vector with negative zero
    CHECK_THROWS_AS(Number(vector<int>{0}, true), invalid_argument);
    CHECK_THROWS_AS(Number(vector<int>{0, 0, 0}, true), invalid_argument);
    // Vector with not base 10 digits
    CHECK_THROWS_AS(Number(vector<int>{10}), invalid_argument);
    CHECK_THROWS_AS(Number(vector<int>{-1}), invalid_argument);
  }

  SECTION("Copy Constructor") {
    Number n1("123");
    Number n2(n1);
    CHECK(n2.is_negative() == false);
    CHECK(n2.get_digits() == vector<int>{3, 2, 1});
  }

  SECTION("Move Constructor") {
    Number n1("123");
    Number n2(move(n1));
    CHECK(n2.is_negative() == false);
    CHECK(n2.get_digits() == vector<int>{3, 2, 1});
  }
}

TEST_CASE("Number Getters", "[getters]") {
  SECTION("is_negative") {
    CHECK(Number("123").is_negative() == false);
    CHECK(Number("-123").is_negative() == true);
  }

  SECTION("get_digits") {
    CHECK(Number("123").get_digits() == vector<int>{3, 2, 1});
    CHECK(Number("-123").get_digits() == vector<int>{3, 2, 1});
  }

  SECTION("to_string") {
    CHECK(Number("123").to_string() == "123");
    CHECK(Number("-123").to_string() == "-123");
  }

  SECTION("size") {
    CHECK(Number("0").size() == 1);
    CHECK(Number("123").size() == 3);
    CHECK(Number("-123").size() == 3);
  }

  SECTION("operator[]") {
    Number n("123");
    CHECK(n[0] == 3);
    CHECK(n[1] == 2);
    CHECK(n[2] == 1);
  }

  SECTION("operator<<") {
    Number n1("123"), n2("-123");
    stringstream s1, s2;
    s1 << n1;
    s2 << n2;
    CHECK(s1.str() == "123");
    CHECK(s2.str() == "-123");
  }
}

TEST_CASE("Number Setters", "[setters]") {
  SECTION("set") {
    Number n;
    n.set("123");
    CHECK(n.is_negative() == false);
    CHECK(n.get_digits() == vector<int>{3, 2, 1});
  }
}

TEST_CASE("Number Multiplication", "[multiplication]") {
  SECTION("fft_multiply") {
    CHECK(fft_multiply(Number("0"), Number("0")).to_string() == "0");
    CHECK(fft_multiply(Number("0"), Number("123")).to_string() == "0");
    CHECK(fft_multiply(Number("123"), Number("0")).to_string() == "0");
    CHECK(fft_multiply(Number("123"), Number("123")).to_string() == "15129");
    CHECK(fft_multiply(Number("-123"), Number("123")).to_string() == "-15129");
    CHECK(fft_multiply(Number("123"), Number("-123")).to_string() == "-15129");
    CHECK(fft_multiply(Number("-123"), Number("-123")).to_string() == "15129");
  }

  SECTION("column_multiply") {
    CHECK(column_multiply(Number("0"), Number("0")).to_string() == "0");
    CHECK(column_multiply(Number("0"), Number("123")).to_string() == "0");
    CHECK(column_multiply(Number("123"), Number("0")).to_string() == "0");
    CHECK(column_multiply(Number("123"), Number("123")).to_string() == "15129");
    CHECK(column_multiply(Number("-123"), Number("123")).to_string() == "-15129");
    CHECK(column_multiply(Number("123"), Number("-123")).to_string() == "-15129");
    CHECK(column_multiply(Number("-123"), Number("-123")).to_string() == "15129");
  }
}

TEST_CASE("Numbers Benchmark", "[!benchmark]") {
  Number a("123456789012345678901234567890");
  Number b("123456789012345678901234567890");

  BENCHMARK("FFT Multiplication") { return fft_multiply(a, b); };

  BENCHMARK("Naive Multiplication") { return column_multiply(a, b); };
}