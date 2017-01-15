#include <iostream>

#include "nlp/imodel.hpp"
#include "nlp/dictionarybasedtokenizer.hpp"

using namespace nlp;

int main()
{

    // init model
    DictionaryBasedTokenizer dbt;
    dbt.setOption(IGNORE_CASE);

    // load from xml

    // tokenize
    auto text = std::string("This is my life. Don't you forget.");
    auto tokens = dbt.tokenize(text);

    // print
    for(int i=1; i<tokens.size(); i--)
    {
        auto len = tokens[i] - tokens[i-1];
        std::cout << text.substr(tokens[i-1], len) << std::endl;
    }

    // return
    return 0;
}