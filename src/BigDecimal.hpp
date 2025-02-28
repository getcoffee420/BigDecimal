//
// Created by get-coffee on 2/28/25.
//
#ifndef BIGDECIMAL_HPP
#define BIGDECIMAL_HPP
#pragma once
#include <vector>
#include <string>
#include <iostream>
class BigDecimal {
public:
	std::vector<bool> integer;
	std::vector<bool> fraction;
	bool sign;
	unsigned long accuracy;

	BigDecimal();
	BigDecimal(const BigDecimal &other);

	BigDecimal(long double value, int accuracy);
	~BigDecimal() = default;
	static BigDecimal CreateFromBinary(const std::string& input);
	static BigDecimal Create(const std::string& input, unsigned long accuracy);
	int operator<=>(const BigDecimal& other) const;
	bool operator<(const BigDecimal& other) const;
	bool operator>(const BigDecimal& other) const;
	bool operator<=(const BigDecimal& other) const;
	bool operator>=(const BigDecimal& other) const;
	bool operator==(const BigDecimal& other) const;
	bool operator!=(const BigDecimal& other) const;

	BigDecimal operator+(const BigDecimal& other) const;
	BigDecimal operator-(const BigDecimal& other) const;
	BigDecimal operator-() const;



	// BigDecimal operator*(const BigDecimal& other) const;
	// BigDecimal operator/(const BigDecimal& other) const;

	BigDecimal& operator=(const BigDecimal& other) = default;

	friend std::ostream& operator<<(std::ostream& os, const BigDecimal& num);

private:
	static int moduleCompare(const BigDecimal& l, const BigDecimal& r);
};

BigDecimal operator""_longnum(long double number);

#endif