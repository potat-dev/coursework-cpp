#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#define CATCH_CONFIG_MAIN

#include "../src/number/number.h"

using namespace std;

TEST_CASE("Numbers Methods", "[number]") {
  SECTION("Constructors") {
    Number n1;
    CHECK(n1.size() == 1);
    CHECK(n1.is_negative() == false);
    CHECK(n1[0] == 0);
    CHECK(n1.to_string() == "0");

    Number n2("123");
    CHECK(n2.size() == 3);
    CHECK(n2.is_negative() == false);
    CHECK((n2[0] == 3 && n2[1] == 2 &&
           n2[2] == 1));  // TODO: add getter for digits
    CHECK(n2.to_string() == "123");

    Number n3("-123");
    CHECK(n3.size() == 3);
    CHECK(n3.is_negative() == true);
    CHECK((n3[0] == 3 && n3[1] == 2 && n3[2] == 1));
    CHECK(n3.to_string() == "-123");

    Number n4(123);
    CHECK(n4.size() == 3);
    CHECK(n4.is_negative() == false);
    CHECK((n4[0] == 3 && n4[1] == 2 && n4[2] == 1));
    CHECK(n4.to_string() == "123");

    Number n5(-123);
    CHECK(n5.size() == 3);
    CHECK(n5.is_negative() == true);
    CHECK((n5[0] == 3 && n5[1] == 2 && n5[2] == 1));
    CHECK(n5.to_string() == "-123");

    Number n6(vector<int>{3, 2, 1});
    CHECK(n6.size() == 3);
    CHECK(n6.is_negative() == false);
    CHECK((n6[0] == 3 && n6[1] == 2 && n6[2] == 1));
    CHECK(n6.to_string() == "123");

    Number n7(vector<int>{3, 2, 1}, true);
    CHECK(n7.size() == 3);
    CHECK(n7.is_negative() == true);
    CHECK((n7[0] == 3 && n7[1] == 2 && n7[2] == 1));
    CHECK(n7.to_string() == "-123");

    // TODO: add tests for invalid arguments
  }

  SECTION("Set") {
    Number n;
    n.set("123");
    CHECK(n.size() == 3);
    CHECK(n.is_negative() == false);
    CHECK((n[0] == 3 && n[1] == 2 && n[2] == 1));
    CHECK(n.to_string() == "123");

    n.set("-123");
    CHECK(n.size() == 3);
    CHECK(n.is_negative() == true);
    CHECK((n[0] == 3 && n[1] == 2 && n[2] == 1));
    CHECK(n.to_string() == "-123");

    n.set("000");
    CHECK(n.size() == 1);
    CHECK(n.is_negative() == false);
    CHECK(n[0] == 0);
    CHECK(n.to_string() == "0");

    n.set("-000");
    CHECK(n.size() == 1);
    CHECK(n.is_negative() == false);
    CHECK(n[0] == 0);
    CHECK(n.to_string() == "0");
  }

  SECTION("Multiplication") {
    Number a("123456789012345678901234567890");
    Number b("123456789012345678901234567890");
    Number c = fft_multiply(a, b);

    REQUIRE(c.to_string() ==
            "15241578753238836750495351562536198787501905199875019052100");
  }
}

TEST_CASE("Numbers Benchmark", "[!benchmark]") {
  Number a("123456789012345678901234567890");
  Number b("123456789012345678901234567890");

  BENCHMARK("FFT Multiplication") { return fft_multiply(a, b); };

  BENCHMARK("Naive Multiplication") { return column_multiply(a, b); };
}