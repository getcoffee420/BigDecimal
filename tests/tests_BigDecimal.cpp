#include <BigDecimal.hpp>
#include <gtest/gtest.h>

TEST(BigDecimalTest, Addition) {
	std::string as, bs;
	as = "3.75";
	bs = "13.75";
	BigDecimal a(10, 128);
	BigDecimal b = BigDecimal::Create(as, 3);
	BigDecimal result = a + b;
	BigDecimal expected = BigDecimal::Create(bs, 3);
	EXPECT_EQ(result, expected);
}

TEST(BigDecimalTest, Equals) {
	BigDecimal a = BigDecimal::Create("123.01", 15);
	BigDecimal b = BigDecimal::Create("123.01", 15);
	EXPECT_EQ(a, b);
}

TEST(BigDecimalTest, AdditionCasual) {
	BigDecimal a = BigDecimal::Create("12345", 0);
	BigDecimal b = BigDecimal::Create("67890", 0);
	BigDecimal result = a + b;
	EXPECT_EQ(result, BigDecimal::Create("80235", 0));
}

TEST(BigDecimalTest, AdditionDifferentSigns) {
	BigDecimal a = BigDecimal::Create("12345", 0);
	BigDecimal b = BigDecimal::Create("-67890", 0);
	BigDecimal result = a + b;
	EXPECT_EQ(result, BigDecimal::Create("-55545", 0));
}

TEST(BigDecimalTest, LongnumEqualsCreate) {
	BigDecimal a = 12.25_longnum;
	BigDecimal b = BigDecimal::Create("12.25", 2);
	EXPECT_EQ(a, b);
}