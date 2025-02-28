//
// Created by get-coffee on 2/28/25.
//
#include "./src/BigDecimal.hpp"

int main(){
	BigDecimal a;
     std::string b;
     std::cin >> b;
     std::cout << BigDecimal::Create(b, 10);
}