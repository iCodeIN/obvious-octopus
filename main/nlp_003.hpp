#include <iostream>

#include "nlp/imodel.hpp"
#include "nlp/dictionarybasedtokenizer.hpp"

using namespace std;
using namespace nlp;

int main()
{

    // init model
    DictionaryBasedTokenizer dbt;
    dbt.setOption(IGNORE_CASE);

    // read xml
    ifstream ifs;
    ifs.open ("dictionary_en.xml", ifstream::in);

    // read model
    auto elementPtr = std::unique_ptr<XML::IElement>(new XML::DefaultElementImpl(""));
    ifs >> *(elementPtr.get());

    // set model
    dbt.fromXML(std::move(elementPtr));

    // tokenize
    auto text = std::string("This is my life. Don't you forget");
    auto tokens = dbt.tokenize(text);
    std::cout << tokens.size() << std::endl;

    // print
    for(int i=1; i < tokens.size(); i++)
    {
        auto len = tokens[i] - tokens[i-1];
        std::cout << i << "\t" <<  text.substr(tokens[i-1], len) << std::endl;
    }

    // return
    return 0;
}