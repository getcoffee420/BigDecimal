//
// Created by get-coffee on 2/28/25.
//
#include "../src/BigDecimal.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cstdint>

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

TEST_CASE("- casual", "[operator-]") {
	BigDecimal a = BigDecimal::Create("50.123", 15);
	BigDecimal b = BigDecimal::Create("40.02", 15);
	BigDecimal result = a + b;
	REQUIRE(result == BigDecimal::Create("10.", 0));
}