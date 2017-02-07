#include <iostream>

#include "nlp/imodel.hpp"
#include "nlp/dictionarybasedtokenizer.hpp"
#include "nlp/metaheuristictokenizer.hpp"
#include "nlp/porterstemmer.hpp"

using namespace std;
using namespace nlp;

int main()
{

    // init model
    auto dbt = std::unique_ptr<DictionaryBasedTokenizer>(new DictionaryBasedTokenizer());
    dbt->setOption(IGNORE_CASE);

    // read xml
    ifstream ifs;
    ifs.open ("dictionary_en.xml", ifstream::in);

    // read model
    auto elementPtr = std::unique_ptr<XML::IElement>(new XML::DefaultElementImpl(""));
    ifs >> *(elementPtr.get());

    // set model
    dbt->fromXML(std::move(elementPtr));

    MetaheuristicTokenizer mht(std::move(dbt));

    // tokenize
    auto text = std::string("Once upon a time I was falling in love, now I'm only falling apart. Nothing I can say, total eclipse of the heart.");
    auto tokens = mht.tokenize(text);
    std::cout << tokens.size() << std::endl;

    // stemmer
    PorterStemmer stm;
    stm.setOption(IGNORE_CASE);

    // print
    for(int i=1; i < tokens.size(); i++)
    {
        auto len = tokens[i] - tokens[i-1];
        auto token = text.substr(tokens[i-1], len);
        auto stem =  stm.stem(token);
        std::cout << i << "\t" <<  token << "\t" << stem << std::endl;
    }

    // return
    return 0;
}