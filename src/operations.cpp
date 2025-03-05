#include "operations.hpp"
#include <algorithm>
#include <cmath>

void timesFive(std::vector<int>& number) {
    int inMind = 0;
    for (int & i : number) {
        int current = i * 5 + inMind;
        i = current % 10;
        inMind = current / 10;
    }
    if (inMind != 0) number.push_back(inMind);
}

void timesTwo(std::vector<int>& number) {
    bool overflow = false;
    for (int & i : number) {
        i *= 2;
        if (overflow) {
            i += 1;
            overflow = false;
        }
        if (i > 9) {
            i -= 10;
            overflow = true;
        }
    }
    if (overflow) number.push_back(1);
}

void plusOne(std::vector<int>& number) {
    for (int & i : number) {
        i += 1;
        if (i != 10) return;
        i = 0;
    }
    number.push_back(1);
}

std::vector<int> plusOneReversed(std::vector<int> number) {
    for (int i = number.size() - 1; i >= 0; i--) {
        number[i] += 1;
        if (number[i] != 10) {
            return number;
        }
        number[i] = 0;
    }
    number.insert(number.begin(), 1);
    return number;
}

void divideByTwo(std::vector<int>& number) {
    int carry = 0;
    for (int i = number.size() - 1; i >= 0; i--) {
        int cur = number[i] + carry * 10;
        number[i] = cur / 2;
        carry = cur % 2;
    }
    if (number.size() > 1 && number.back() == 0) {
        number.pop_back();
    }
}

std::vector<int> toDec(const std::vector<bool>& number) {
    std::vector<int> result = {0};
    std::vector<bool> temp(number.size());
    for (int i = 0; i < number.size(); i++) {
        temp[i] = number[number.size() - 1 - i];
    }
    for (bool bit : temp) {
        timesTwo(result);
        if (bit) plusOne(result);
    }
    return result;
}

std::vector<bool> toBin(std::vector<int> number) {
    std::vector<bool> result;

    if (number.size() == 1 && number[0] == 0) {
        return {0};
    }

    while (!(number.size() == 1 && number[0] == 0)) {
        result.push_back(number[0] % 2);
        divideByTwo(number);
    }

    std::reverse(result.begin(), result.end());
    return result;
}

bool round(std::vector<int>& number, const int binAcc) {
    bool numberOverflow = false;
    int decAcc = std::ceil(binAcc * 0.30103);
    if (decAcc > 1) decAcc -= 1;
    std::ranges::reverse(number);
    while (number.size() > decAcc) {
        bool overflow = number[number.size() - 1] > 4;
        number.pop_back();
        if (overflow) {
            int cur = number.size();
            plusOneReversed(number);
            if (cur != number.size()) {
                numberOverflow = true;
            }
        };
    }
    while (number.size() < decAcc) {
        number.push_back(0);
    }
    std::ranges::reverse(number);
    return numberOverflow;
}

std::vector<bool> toBinFraction(std::vector<int> decFrac, const unsigned long accuracy) {
    std::vector<bool> result;
    unsigned long siz = decFrac.size();
    for (int i = 0; i < accuracy; i++) {
        timesTwo(decFrac);
        if (decFrac.size() > siz) {
            decFrac.pop_back();
            result.push_back(true);
        } else {
            result.push_back(false);
        }
    }
    std::reverse(result.begin(), result.end());
    return result;
}