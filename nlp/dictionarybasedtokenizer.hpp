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
                tokenBoundaries.push_back(0);

                for(int i=0; i<s.size(); i++)
                {
                    auto token = s.substr(tokenBoundaries[tokenBoundaries.size()-1], i);
                    // if the token is a valid prefix, continue building the token
                    if(isPrefix(token))
                    {
                        continue;
                    }
                    // if the token is a word, mark the boundary
                    else if(isWord(token))
                    {
                        tokenBoundaries.push_back(i);
                        continue;
                    }
                    // unhappy flow
                    else
                    {
                        if(token.size() == 1)
                        {
                            tokenBoundaries.push_back(i - 1);
                            i--;
                        }
                        else
                        {
                            auto prevToken = token.substr(0, token.size() - 1);
                            if(isPrefix(prevToken) || isWord(prevToken))
                            {
                                tokenBoundaries.push_back(i - 1);
                            }
                            else
                            {

                            }
                        }
                    }
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
            bool isPrefix(const std::string& s) const
            {
                return m_prefixes.find(s) != m_prefixes.cend();
            }
            bool isWord(const std::string& s) const
            {
                return m_words.find(s) != m_words.cend();
            }
            // --- members ---
            std::set<std::string> m_words;
            std::set<std::string> m_prefixes;
    };
}

#endif // DICTIONARYBASEDTOKENIZER_HPP