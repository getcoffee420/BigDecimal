//
// Created by get-coffee on 2/28/25.
//

#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP
#pragma once
#include <vector>

void timesFive(std::vector<int>& number);
void timesTwo(std::vector<int>& number);
void plusOne(std::vector<int>& number);
std::vector<int> plusOneReversed(std::vector<int> number);
void divideByTwo(std::vector<int>& number);
std::vector<int> toDec(const std::vector<bool>& number);
std::vector<bool> toBin(std::vector<int> number);
bool round(std::vector<int>& number, int binAcc);
std::vector<bool> toBinFraction(std::vector<int> decFrac, unsigned long accuracy);

#endif // OPERATIONS_HPP