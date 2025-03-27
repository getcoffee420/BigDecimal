#include <BigDecimal.hpp>
#include <gtest/gtest.h>

void printBigDecimalDetails(const BigDecimal& num) {
    std::cout << "Sign: " << (num.sign ? "Positive" : "Negative") << std::endl;
    std::cout << "Accuracy: " << num.accuracy << std::endl;

    std::cout << "Integer part (binary): ";
    if (num.integer.empty()) {
        std::cout << "Empty";
    } else {
        for (bool bit : num.integer) {
            std::cout << bit;
        }
    }
    std::cout << std::endl;

    std::cout << "Fraction part (binary): ";
    if (num.fraction.empty()) {
        std::cout << "Empty";
    } else {
        for (bool bit : num.fraction) {
            std::cout << bit;
        }
    }
    std::cout << std::endl;

    std::cout << "-------------------------" << std::endl;
}


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

TEST(BigDecimalTest, LongnumZeroCreate) {
	BigDecimal a = 0.0_longnum;
	BigDecimal b = BigDecimal::Create("0", 2);
	EXPECT_EQ(a, b);
}

TEST(BigDecimalTest, MultiplyTest) {
	BigDecimal a = BigDecimal::Create("2.5", 41);
	BigDecimal b = BigDecimal::Create("4.5", 41);
	EXPECT_EQ(a * b, BigDecimal::Create("11.25", 3));
}

TEST(BigDecimalTest, MultiplyDiffSignsTest) {
	BigDecimal a = BigDecimal::Create("2.5", 10);
	BigDecimal b = BigDecimal::Create("-4.5", 10);
	EXPECT_EQ(a * b, BigDecimal::Create("-11.25", 10));
}