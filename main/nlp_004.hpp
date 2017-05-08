#include "nlp/dictionarybasedtokenizer.hpp"
#include "nlp/imodel.hpp"
#include "nlp/metaheuristictokenizer.hpp"
#include "nlp/porterstemmer.hpp"

#include <iostream>

using namespace std;
using namespace nlp;

std::unique_ptr<Dictionary> loadDictionary()
{
    // read xml
    ifstream ifs;
    ifs.open ("dictionary_en.xml", ifstream::in);

    // read model
    auto elementPtr = std::unique_ptr<XML::IElement>(new XML::DefaultElementImpl(""));
    ifs >> *(elementPtr.get());

    // init dictionary
    auto dPtr = std::unique_ptr<Dictionary>(new Dictionary());
    dPtr->setOption(IGNORE_CASE);
    dPtr->fromXML(std::move(elementPtr));

    return std::move(dPtr);
}

int main()
{

    // init tokenizer
    auto dbt = std::unique_ptr<DictionaryBasedTokenizer>(new DictionaryBasedTokenizer(loadDictionary()));
    dbt->setOption(IGNORE_CASE);

    // init metaheuristic tokenizer
    MetaheuristicTokenizer mht(std::move(dbt), loadDictionary());

    // tokenize
    std::cout << "tokenization : " << std::endl;
    auto text = std::string("THIS IS AN EXBMPLE SENTENCE.");
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