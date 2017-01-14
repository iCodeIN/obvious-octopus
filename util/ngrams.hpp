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
            static std::map<std::string, int> ngrams(const std::string& s, std::vector<int> ngramsSizes, bool ignoreCase, bool ignoreNonAlphaNumeric)
            {
                std::map<std::string,int> count;
                for(int i=0; i<s.size(); i++)
                {
                    for(int l : ngramsSizes)
                    {
                        if(i+l <= s.size())
                        {
                            if(ignoreNonAlphaNumeric && containsNonAlphaNumeric(s, i, i + l))
                            {
                                continue;
                            }
                            if(ignoreCase)
                            {
                                count[toUpper(s, i, i + l)]++;
                            }
                            else
                            {
                                count[s.substr(i, l)]++;
                            }
                        }
                    }
                }
                return count;
            }

        private:
            // --- methods ---

            static std::string toUpper(const std::string& s, int from, int to)
            {
                std::string ret;
                for(int i=from; i<to; i++)
                {
                    ret += toupper(s[i]);
                }
                return ret;
            }

            static bool containsNonAlphaNumeric(const std::string& s, int from, int to)
            {
                for(int i=from; i<to; i++)
                {
                    if(!std::isalnum(s[i]))
                    {
                        return true;
                    }
                }
                return false;
            }

            // --- members ---
    };
}

#endif // NGRAMS_HPP