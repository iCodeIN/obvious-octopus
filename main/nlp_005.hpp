#include "nlp/dictionary.hpp"

#include <iostream>

using namespace nlp;

int main()
{
    Dictionary dict;
    dict.insert("lorem");

    // prefixes
    std::cout << "Testing prefixes" << std::endl;
    std::cout << dict.isPrefix("") << std::endl;
    std::cout << dict.isPrefix("l") << std::endl;
    std::cout << dict.isPrefix("lo") << std::endl;
    std::cout << dict.isPrefix("lor") << std::endl;
    std::cout << dict.isPrefix("lore") << std::endl;
    std::cout << dict.isPrefix("lorem") << std::endl;

    // suffixes
    std::cout << "Testing suffixes" << std::endl;
    std::cout << dict.isSuffix("") << std::endl;
    std::cout << dict.isSuffix("m") << std::endl;
    std::cout << dict.isSuffix("em") << std::endl;
    std::cout << dict.isSuffix("rem") << std::endl;
    std::cout << dict.isSuffix("orem") << std::endl;
    std::cout << dict.isSuffix("lorem") << std::endl;

    // pseudo words
    std::cout << "Testing pseudo words" << std::endl;
    std::cout << dict.isPseudoWord("loram",1) << std::endl;

    // exact match
    std::cout << "Testing exact match" << std::endl;
    std::cout << dict.isWord("lorem") << std::endl;


    // return
    return 0;
}