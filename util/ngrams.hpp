#ifndef NGRAMS_HPP
#define NGRAMS_HPP

#include <map>
#include <string>
#include <vector>
#include <iostream> // debug

namespace util
{
    /*! In the fields of computational linguistics and probability, an n-gram is a contiguous sequence of n items from a given sequence of text or speech.
        The items can be phonemes, syllables, letters, words or base pairs according to the application.
        The n-grams typically are collected from a text or speech corpus. When the items are words, n-grams may also be called shingles.
     */
    class NGrams
    {
        public:

            /*! Prohibit construction of NGrams. This class offers only static methods.
             */
            explicit NGrams() = delete;

            /*! \return a frequency-map of all ngrams in the given std:string
                \param[in] s            the string to extract ngrams from
                \param[in] ngramsSizes  the sizes of the ngrams to be extracted
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
                            if(ignoreNonAlpha && containsNonAlpha(s, i, i + l))
                            {
                                continue;
                            }
                            if(ignoreCase)
                            {
                                count[ignoreCase ? Case::toUpper(ngram) : ngram]++;
                            }
                        }
                    }
                }
                return count;
            }

        private:
            // --- methods ---

            static bool containsNonAlpha(const std::string& s, int from, int to)
            {
                for(int i=from; i<to; i++)
                {

                    if(!std::isalpha(s[i]) && !std::isspace(s[i]))
                    {
                        return true;
                    }
                    /*
                    if(std::ispunct(s[i]) || std::iscntrl(s[i]) || std::isdigit(s[i]))
                    {
                        return true;
                    }
                    */
                }
                return false;
            }

            // --- members ---
    };
}

#endif // NGRAMS_HPP