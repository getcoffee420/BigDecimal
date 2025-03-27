#include "BigDecimal.hpp"
#include "./operations.hpp"
#include <algorithm>
#include <cmath>
#include <cassert>

BigDecimal::BigDecimal() {
    integer.resize(1);
    integer[0] = 0;
    fraction.resize(1);
    fraction[0] = 0;
    sign = true;
    accuracy = 1;
}

BigDecimal::BigDecimal(const BigDecimal &other) {
    integer = other.integer;
    fraction = other.fraction;
    sign = other.sign;
    accuracy = other.accuracy;
}

BigDecimal::BigDecimal(long double value, int accuracy) {
    this->accuracy = accuracy;
    this->sign = value >= 0;
    if (value < 0) value = -value;

    if (value == 0.0) {
        // Ноль
        integer.resize(0);
        fraction.resize(0);
        return;
    }

    int a = std::floor(value);
    value -= std::floor(value);

    std::vector<bool> digits;
    while (a > 0) {
        digits.push_back(a % 2);
        a /= 2;
    }
    this->integer = digits;

    std::vector<bool> fracDigits;
    this->fraction.resize(0);
    for (int i = 0; i < accuracy; i++) {
        value *= 2;
        fracDigits.push_back(value >= 1);
        if (value >= 1) value -= 1;
    }
    std::reverse(fracDigits.begin(), fracDigits.end());
    this->fraction = fracDigits;
}

BigDecimal BigDecimal::CreateFromBinary(const std::string& input) {
    BigDecimal result = BigDecimal();
    int curPos = 0;
    if(input[0] == '-') {
        result.sign = false;
        curPos = 1;
    } else if(input[0] == '+') {
        result.sign = true;
        curPos = 1;
    } else {
        result.sign = true;
    }

    std::vector<int> digits;
    for(; curPos < input.size(); curPos++) {
        if(input[curPos] == '.' or input[curPos] == ',') {
            break;
        }
        digits.push_back(input[curPos] - '0');
    }
    std::vector<int> fractionDigits;
    for(curPos += 1; curPos < input.size(); curPos++) {
        fractionDigits.push_back(input[curPos] - '0');
    }
    result.fraction.resize(fractionDigits.size());
    for(curPos = 0; curPos < fractionDigits.size(); curPos++) {
        result.fraction[curPos] = fractionDigits[fractionDigits.size() - 1 - curPos];
    }
    result.integer.resize(digits.size());
    for(curPos = 0; curPos < digits.size(); curPos++) {
        result.integer[curPos] = digits[digits.size() - 1 - curPos];
    }
    result.accuracy = result.fraction.size();
    return result;
}

BigDecimal BigDecimal::Create(const std::string& input, unsigned long accuracy) {
    BigDecimal result = BigDecimal();
    result.accuracy = accuracy;

    int curPos = 0;
    if (input[0] == '-') {
        result.sign = false;
        curPos = 1;
    } else if (input[0] == '+') {
        result.sign = true;
        curPos = 1;
    } else {
        result.sign = true;
    }

    std::vector<int> digits;
    for (; curPos < input.size(); curPos++) {
        if (input[curPos] == '.' || input[curPos] == ',') {
            break;
        }
        digits.push_back(input[curPos] - '0');
    }

    std::vector<int> fractionDigits;
    for (curPos += 1; curPos < input.size() && fractionDigits.size() < accuracy; curPos++) {
        fractionDigits.push_back(input[curPos] - '0');
    }

    if (digits.empty() && fractionDigits.empty()) {
        result.integer.resize(0);
        result.fraction.resize(0);
        result.sign = true;
        return result;
    }

    std::reverse(fractionDigits.begin(), fractionDigits.end());
    result.fraction = toBinFraction(fractionDigits, accuracy);

    std::reverse(digits.begin(), digits.end());
    std::vector<bool> boolDigits = toBin(digits);
    result.integer.resize(boolDigits.size());
    for (size_t i = 0; i < boolDigits.size(); i++) {
        result.integer[i] = boolDigits[boolDigits.size() - 1 - i];
    }

    return result;
}

int BigDecimal::operator<=>(const BigDecimal& other) const {
  	int temp = moduleCompare(*this, other);
    if(temp == 0) return 0;
    if(this->sign != other.sign) {

        if(this->sign == true) return 1;
        else return -1;
    }
    if(this->sign) {

        return temp;
    } else return -1 * temp;
}

bool BigDecimal::operator<(const BigDecimal& other) const {
    return operator<=>(other) < 0;
}

bool BigDecimal::operator>(const BigDecimal& other) const {
    return operator<=>(other) > 0;
}

bool BigDecimal::operator<=(const BigDecimal& other) const {
    return operator<=>(other) <= 0;
}

bool BigDecimal::operator>=(const BigDecimal& other) const {
    return operator<=>(other) >= 0;
}

bool BigDecimal::operator==(const BigDecimal& other) const {
    return operator<=>(other) == 0;
}

bool BigDecimal::operator!=(const BigDecimal& other) const {
    return operator<=>(other) != 0;
}

void BigDecimal::divideByTwoBin() {
    if (!integer.empty()) {
        fraction.push_back(integer[0]);
        accuracy++;
    }

    if(integer.empty()) {
        fraction.push_back(0);
    }

    if (!integer.empty()) {
        std::reverse(integer.begin(), integer.end());
        integer.pop_back();
        std::reverse(integer.begin(), integer.end());
    }
}

void BigDecimal::timesTwoBin() {
    if (!fraction.empty()) {
        integer.insert(integer.begin(), fraction.back());
        fraction.pop_back();
        accuracy--;
    }
    else {
        integer.insert(integer.begin(), 0);
    }
}

BigDecimal BigDecimal::operator+(const BigDecimal& other) const {
    if(sign != other.sign) {
        BigDecimal otherCopy = other;
        otherCopy.sign = !other.sign;
        return operator-(otherCopy);
    }
    BigDecimal result = *this;
    result.sign = sign;
    result.accuracy = std::ranges::max(this->accuracy, other.accuracy);
    bool carry = false;

    result.fraction.resize(result.accuracy, false);
    for (size_t i = 0; i < result.accuracy; i++) {
        const bool leftValue = i >= result.accuracy - this->accuracy ? this->fraction[i - (result.accuracy - this->accuracy)] : false;
        const bool rightValue = i >= result.accuracy - other.accuracy ? other.fraction[i - (result.accuracy - other.accuracy)] : false;
        int temp = static_cast<int>(leftValue) + static_cast<int>(rightValue) + static_cast<int>(carry);
        carry = temp > 1;
        result.fraction[i] = temp % 2;
    }

    result.integer.resize(std::max(integer.size(), other.integer.size()), false);
    for (size_t i = 0; i < std::max(integer.size(), other.integer.size()); i++) {
        bool leftValue = (i < integer.size()) ? integer[i] : false;
        bool rightValue = (i < other.integer.size()) ? other.integer[i] : false;
        int temp = static_cast<int>(leftValue) + static_cast<int>(rightValue) + static_cast<int>(carry);
        result.integer[i] = temp % 2;
        carry = temp > 1;
    }

    if (carry) {
        result.integer.push_back(true);
    }

    return result;
}

BigDecimal BigDecimal::operator-() const {
    BigDecimal result = *this;
    result.sign = !sign;
    return result;
}

BigDecimal BigDecimal::operator-(const BigDecimal& other) const {
    if(sign != other.sign) {
        BigDecimal otherCopy = other;
        otherCopy.sign = !other.sign;
        return operator+(otherCopy);
    }

    if (moduleCompare(*this, other) < 0) {
        return -(other - *this);
    }

    BigDecimal result = *this;
    result.sign = sign;
    result.accuracy = std::max(this->accuracy, other.accuracy);
    bool borrow = false;

    result.fraction.resize(result.accuracy, false);
    for (size_t i = 0; i < result.accuracy; i++) {
        bool leftValue = i >= result.accuracy - this->accuracy ? this->fraction[i - (result.accuracy - this->accuracy)] : false;
        bool rightValue = i >= result.accuracy - other.accuracy ? other.fraction[i - (result.accuracy - other.accuracy)] : false;
        int temp = static_cast<int>(leftValue) - static_cast<int>(rightValue) - static_cast<int>(borrow);
        borrow = temp < 0;
        result.fraction[i] = borrow ? temp + 2 : temp;
    }

    result.integer.resize(std::max(integer.size(), other.integer.size()), false);
    for (size_t i = 0; i < result.integer.size(); i++) {
        bool leftValue = (i < integer.size()) ? integer[i] : false;
        bool rightValue = (i < other.integer.size()) ? other.integer[i] : false;
        int temp = static_cast<int>(leftValue) - static_cast<int>(rightValue) - static_cast<int>(borrow);
        borrow = temp < 0;
        result.integer[i] = borrow ? temp + 2 : temp;
    }

    while (result.integer.size() > 1 && !result.integer.back()) {
        result.integer.pop_back();
    }

    return result;
}

BigDecimal BigDecimal::operator*(const BigDecimal& other) const {
    BigDecimal result;

    result.sign = (this->sign == other.sign);
    result.accuracy = accuracy + other.accuracy;

    BigDecimal temp = *this;
    BigDecimal multiplier = other;

    BigDecimal resultFraction;
    resultFraction.sign = result.sign;
    resultFraction.accuracy = result.accuracy;

    for (size_t i = 0; i < multiplier.fraction.size(); ++i) {
        if (multiplier.fraction[multiplier.accuracy - i]) {
            resultFraction = resultFraction + temp;
        }
        temp.divideByTwoBin();
    }

    BigDecimal resultInteger;
    resultInteger.sign = result.sign;

    temp = *this;
    for (size_t i = 0; i < multiplier.integer.size(); ++i) {
        if (multiplier.integer[i]) {
            resultInteger = resultInteger + temp;
        }
        temp.timesTwoBin();
    }

    result = resultFraction + resultInteger;
    result.fraction.resize(result.accuracy);
    result.sign = (this->sign == other.sign);
    return result;
}

BigDecimal operator""_longnum(long double number) {
    return BigDecimal(number, 64);
}

int BigDecimal::moduleCompare(const BigDecimal &l, const BigDecimal &r) {
    bool lIsZero = (l.integer.empty() || std::all_of(l.integer.begin(), l.integer.end(), [](bool bit) { return !bit; })) &&
                   (l.fraction.empty() || std::all_of(l.fraction.begin(), l.fraction.end(), [](bool bit) { return !bit; }));
    bool rIsZero = (r.integer.empty() || std::all_of(r.integer.begin(), r.integer.end(), [](bool bit) { return !bit; })) &&
                   (r.fraction.empty() || std::all_of(r.fraction.begin(), r.fraction.end(), [](bool bit) { return !bit; }));

    if (lIsZero && rIsZero) {
        return 0;
    }

    if (l.integer.size() != r.integer.size()) {
        return l.integer.size() > r.integer.size() ? 1 : -1;
    }
    for (int i = static_cast<int>(l.integer.size()) - 1; i >= 0; i--) {
        if (l.integer[i] != r.integer[i]) {
            return l.integer[i] > r.integer[i] ? 1 : -1;
        }
    }

    size_t maxAccuracy = std::max(l.accuracy, r.accuracy);
    for (size_t i = 0; i < maxAccuracy; i++) {
        bool lBit = i < l.accuracy ? l.fraction[l.accuracy - 1 - i] : false;
        bool rBit = i < r.accuracy ? r.fraction[r.accuracy - 1 - i] : false;
        if (lBit != rBit) {
            return lBit > rBit ? 1 : -1;
        }
    }

    return 0;
}

std::ostream& operator<<(std::ostream& os, const BigDecimal& num) {
    if(num.sign == false) os << '-';

    std::vector<int> number = toDec(num.integer);
    std::vector<int> fraction = toDec(num.fraction);

    if (!num.fraction.empty()) {
        for (size_t i = 0; i < num.fraction.size(); i++) {
            timesFive(fraction);
        }
    }

    unsigned long zeros = num.fraction.size() - fraction.size();

    if(round(fraction, num.accuracy)) {
        if(zeros == 0) {
            plusOne(number);
        }
        else {
            zeros--;
            fraction.push_back(1);
        }
    }

    if(number.empty()) os << 0;
    for(int i = 0; i < number.size(); i++) {
        os << number[number.size() - i - 1];
    }

    if(!num.fraction.empty()) {
        os << '.';

        for (int i = 0; i < zeros; i++) {
            os << '0';
        }
        for (int i = static_cast<int>(fraction.size()) - 1; i >= 0; i--) {
            os << fraction[i];
        }
    }
    return os;
}