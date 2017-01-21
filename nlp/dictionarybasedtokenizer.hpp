#ifndef DICTIONARYBASEDTOKENIZER_HPP
#define DICTIONARYBASEDTOKENIZER_HPP

#include <set>

#include "nlp/itokenizer.h"
#include "util/case.hpp"
#include "xml/xml.hpp"

namespace nlp
{
    /*! ITokenizer implementation using a dictionary-based algorithm
     */
    class DictionaryBasedTokenizer : public ITokenizer
    {
        public:

            /*! default constructor
             */
            explicit DictionaryBasedTokenizer()
            {
            }

            /*! default destructor
             */
            virtual ~DictionaryBasedTokenizer() = default;

            //! --- ITokenizer ---
            std::vector<int> tokenize(const std::string& s) const override
            {
                std::vector<int> tokenBoundaries;
                tokenBoundaries.push_back(0);

                for(int i=0; i<s.size(); i++)
                {
                    auto len = i - tokenBoundaries[tokenBoundaries.size() - 1];
                    auto token = s.substr(tokenBoundaries[tokenBoundaries.size()-1], len);
                    if(this->isSet(IGNORE_CASE))
                    {
                        token = util::Case::toUpper(token);
                    }

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
                        /*  The code that follows relies on there being a prefix of the current token
                            if that is not the case, we deal with it here.
                         */
                        if(token.size() == 1)
                        {
                            tokenBoundaries.push_back(i);
                        }
                        else
                        {
                            /*  If the prefix of the current token is valid prefix (or better, a word)
                                we mark the boundary, and go back one character.
                             */
                            auto prevToken = token.substr(0, token.size() - 1);
                            if(isPrefix(prevToken) || isWord(prevToken))
                            {
                                tokenBoundaries.push_back(i - 1);
                                i--;
                            }
                            else
                            {
                                tokenBoundaries.push_back(i);
                            }
                        }
                    }
                }

                // ensure that the end of the string is always marked as a boundary
                if(tokenBoundaries[tokenBoundaries.size() - 1] != s.size())
                {
                    tokenBoundaries.push_back(s.size());
                }

                // return
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
                    if(this->isSet(IGNORE_CASE))
                    {
                        word = util::Case::toUpper(word);
                    }
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