#include "BigDecimal.hpp"
#include "./operations.hpp"
#include <algorithm>
#include <cmath>

BigDecimal::BigDecimal() {
    integer.resize(0);
    fraction.resize(0);
    sign = true;
    accuracy = 0;
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
    if(value < 0) value = -value;
    int a = std::floor(value);
    value -= std::floor(value);
    std::vector <bool> digits;
    while (a > 0) {
        digits.push_back(a % 2);
        a /= 2;
    }
    this->integer = digits;

    std::vector <bool> fracDigits;
    this->fraction.resize(0);
    for(int i = 0; i < accuracy; i++) {
        value *= 2;
        fracDigits.push_back(value >= 1);
        if(value >= 1) value -= 1;
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

    std::reverse(fractionDigits.begin(), fractionDigits.end());
    result.fraction = toBinFraction(fractionDigits, accuracy);

    std::reverse(digits.begin(), digits.end());
    std::vector<bool> boolDigits = toBin(digits);
    result.integer.resize(boolDigits.size());
    for(curPos = 0; curPos < boolDigits.size(); curPos++) {
        result.integer[curPos] = boolDigits[boolDigits.size() - 1 - curPos];
    }
    return result;
}

int BigDecimal::operator<=>(const BigDecimal& other) const {
    if(this->sign != other.sign) {
        if(this->sign == true) return 1;
        else return -1;
    }
    if(this->sign) {
        return moduleCompare(*this, other);
    } else return -1 * moduleCompare(*this, other);
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

BigDecimal operator""_longnum(long double number) {
    return BigDecimal(number, 64);
}



int BigDecimal::moduleCompare(const BigDecimal &l, const BigDecimal &r) {
        if(l.integer.size() != r.integer.size()) {
            if(l.integer.size() > r.integer.size()) {
                return 1;
            } else return -1;
        }
        for(int i = l.integer.size() - 1; i >= 0; i--) {
            if(l.integer[i] != r.integer[i]) {
                if( l.integer[i] > r.integer[i]) {
                    return 1;
                } else return -1;
            }
        }
        for(int i = 0; i < std::ranges::min(l.accuracy, r.accuracy); i++) {
            if(l.fraction[l.accuracy - 1 - i] != r.fraction[r.accuracy - 1 - i]) {
                if (l.fraction[l.accuracy-1-i] > r.fraction[r.accuracy-1-i]) {
                    return 1;
                } else return -1;
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