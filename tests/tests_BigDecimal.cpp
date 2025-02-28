//
// Created by get-coffee on 2/28/25.
//
#include "../src/BigDecimal.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cstdint>

TEST_CASE( "Factorials are computed", "[factorial]" ) {
	REQUIRE( factorial( 1) == 1 );
	REQUIRE( factorial( 2) == 2 );
	REQUIRE( factorial( 3) == 6 );
	REQUIRE( factorial(10) == 3'628'800 );
}