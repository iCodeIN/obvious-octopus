#pragma once
#ifndef NLP_DICTIONARYBASEDTOKENIZER_HPP
#define NLP_DICTIONARYBASEDTOKENIZER_HPP

#include "nlp/dictionary.hpp"
#include "nlp/itokenizer.h"
#include "util/stringutils.hpp"
#include "xml/xml.hpp"

#include <memory>
#include <set>

namespace nlp
{
    /*! ITokenizer implementation using a dictionary-based algorithm
     */
    class DictionaryBasedTokenizer : public ITokenizer
    {
        public:

            /*! default constructor
             */
            explicit DictionaryBasedTokenizer(std::unique_ptr<Dictionary> dictionary)
                : m_dictionary(std::move(dictionary))
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

                for(int i=0; i <= s.size(); i++)
                {
                    auto len = i - tokenBoundaries[tokenBoundaries.size() - 1];
                    auto token = s.substr(tokenBoundaries[tokenBoundaries.size()-1], len);
                    if(this->isSet(IGNORE_CASE))
                    {
                        token = util::string::toUpper(token);
                    }

                    // if the token is a valid prefix, continue building the token
                    if(m_dictionary->isPrefix(token))
                    {
                        // std::cout << token << ", pos : " << i << ", valid prefix, continue" << std::endl;
                        continue;
                    }
                    // if the token is a word, mark the boundary
                    else if(m_dictionary->isWord(token))
                    {
                        // std::cout << token << ", pos : " << i << ", valid word, push boundary" << std::endl;
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
                            // std::cout << token << ", pos : " << i << ", single char token, push boundary" << std::endl;
                            tokenBoundaries.push_back(i);
                        }
                        else
                        {
                            /*  If the prefix of the current token is valid prefix (or better, a word)
                                we mark the boundary, and go back one character.
                             */
                            auto prevToken = token.substr(0, token.size() - 1);
                            if(m_dictionary->isPrefix(prevToken) || m_dictionary->isWord(prevToken))
                            {
                                // std::cout << token << ", pos : " << i << ", prev token was valid prefix, or valid word, push boundary, resume from " << (i-1) << std::endl;
                                tokenBoundaries.push_back(i - 1);
                                i--;
                            }
                            else
                            {
                                // std::cout << token << ", pos : " << i << ", no useful information, push boundary" << std::endl;
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

        private:

            // --- members ---
            std::unique_ptr<Dictionary> m_dictionary;
    };
}

#endif // NLP_DICTIONARYBASEDTOKENIZER_HPP