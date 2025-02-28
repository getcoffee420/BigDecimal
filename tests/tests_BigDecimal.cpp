//
// Created by get-coffee on 2/28/25.
//
#include "../src/BigDecimal.hpp"
#include <catch2/catch_test_macros.hpp>

#include <cstdint>

TEST_CASE("equals", "[operator==]") {
	BigDecimal a = BigDecimal::Create("123.01", 15);
	BigDecimal b = BigDecimal::Create("123.01", 15);
	REQUIRE(a == b);
}

TEST_CASE("comparison different sign", "[operator<=>]") {
	BigDecimal a = 123.25_longnum;
	BigDecimal b = -44.5_longnum;
	REQUIRE(a > b && b < a && a >= b && b <= a && a != b);
}

TEST_CASE("comparison same sign", "[operator<=>]") {
	BigDecimal a = 123.25_longnum;
	BigDecimal b = 44.5_longnum;
	REQUIRE(a > b && b < a && a >= b && b <= a && a != b);
}

TEST_CASE("+ casual", "[operator+]") {
	BigDecimal a = BigDecimal::Create("12345", 0);
	BigDecimal b = BigDecimal::Create("67890", 0);
	BigDecimal result = a + b;
	REQUIRE(result == BigDecimal::Create("80235", 0));
}

TEST_CASE("+ different signs", "[operator+]") {
	BigDecimal a = BigDecimal::Create("12345", 0);
	BigDecimal b = BigDecimal::Create("-67890", 0);
	BigDecimal result = a + b;
	REQUIRE(result == BigDecimal::Create("-55545", 0));
}

TEST_CASE("_longnum is equal to Create", "[_longnum]") {
	BigDecimal a = 12.25_longnum;
	BigDecimal b = BigDecimal::Create("12.25", 2);
	REQUIRE(a == b);
}