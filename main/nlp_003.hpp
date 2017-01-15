#include <iostream>

#include "nlp/imodel.hpp"
#include "nlp/dictionarybasedtokenizer.hpp"

using namespace nlp;

int main()
{

    DictionaryBasedTokenizer dbt;
    dbt.setOption(IGNORE_CASE);

    auto tokens = dbt.tokenize("This is my life. Don't you forget.");

    // return
    return 0;
}