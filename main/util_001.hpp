#include "util/morse.hpp"

#include <iostream>

int main()
{
    std::string s = "I love you";
    std::cout << util::string::toMorse(s) << std::endl;
    return 0;
}