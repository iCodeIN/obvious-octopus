#include <iostream>

#include "util/levenshtein.hpp"

int main()
{
    std::cout << util::string::levenshtein("banana","elephant") << std::endl;
}