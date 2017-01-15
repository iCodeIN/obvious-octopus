#ifndef DICTIONARYBASEDTOKENIZER_HPP
#define DICTIONARYBASEDTOKENIZER_HPP

#include <set>

#include "nlp/itokenizer.h"
#include "xml/xml.hpp"

namespace nlp
{
    /*! ITokenizer implementation using a dictionary-based algorithm
     */
    class DictionaryBasedTokenizer : public ITokenizer
    {
        public:

            /*! constructor
             */
            explicit DictionaryBasedTokenizer()
            {
            }

            /*! destructor
             */
            virtual ~DictionaryBasedTokenizer() = default;

            // --- ITokenizer ---
            std::vector<int> tokenize(std::string s) const override
            {
                std::vector<int> tokenBoundaries;
                for(int i=0; i<s.size(); i++)
                {

                }
                return tokenBoundaries;
            }

            /*! load a DictionaryBasedTokenizer from a given XML element
             */
            virtual void fromXML(std::unique_ptr<XML::IElement> xml)
            {
                // root element

                // elements for each observed language
                for(int i=0; i<xml->countChildren(); i++)
                {
                    auto word = xml->getChild(i).getText();
                    m_words.insert(word);
                    for(int j=0; j<word.size(); j++)
                    {
                        auto prefix = word.substr(0, j);
                        m_prefixes.insert(prefix);
                    }
                }
            }

        private:
            // --- methods ---
            // --- members ---
            std::set<std::string> m_words;
            std::set<std::string> m_prefixes;
    };
}

#endif // DICTIONARYBASEDTOKENIZER_HPP