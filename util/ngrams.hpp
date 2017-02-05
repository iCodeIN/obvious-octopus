#ifndef NGRAMS_HPP
#define NGRAMS_HPP

#include "util/stringutils.hpp"

#include <map>
#include <string>
#include <vector>
#include <iostream> // debug

namespace util
{
    namespace string
    {
        /*! \return a frequency-map of all n-grams in the given std:string
            \param[in] s                the string to extract ngrams from
            \param[in] maxNGramSize     the maximum length (number of characters) of the ngrams to be extracted
            \param[in] ignoreCase       whether case-information is ignored or not when counting n-grams
            \param[in] ignoreNonAlpha   whether n-grams containing non-alpha characters are ignored or not
         */
        static std::map<std::string, int> ngrams(const std::string& s, int maxNGramSize, bool ignoreCase, bool ignoreNonAlpha)
        {
            std::map<std::string,int> count;
            for(int i=0; i<s.size(); i++)
            {
                for(int l = 1 ; l <= maxNGramSize ; l++)
                {
                    if(i + l <= s.size())
                    {
                        auto ngram = s.substr(i, i + l);
                        if(ignoreNonAlpha && hasNonAlpha(ngram))
                        {
                            continue;
                        }
                        if(ignoreCase)
                        {
                            count[ignoreCase ? toUpper(ngram) : ngram]++;
                        }
                    }
                }
            }
            return count;
        }
    }
}

#endif // NGRAMS_HPP