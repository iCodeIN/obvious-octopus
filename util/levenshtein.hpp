#pragma once
#ifndef UTIL_LEVENSHTEIN_HPP
#define UTIL_LEVENSHTEIN_HPP

#include <string>
#include <vector>

namespace util
{
    namespace string
    {
        /*! In information theory and computer science, the Levenshtein distance is a string metric for measuring the difference between two sequences.
            Informally, the Levenshtein distance between two words is the minimum number of single-character edits (i.e. insertions, deletions or substitutions) required to change one word into the other.
            It is named after Vladimir Levenshtein, who considered this distance in 1965.
            Levenshtein distance may also be referred to as edit distance, although that may also denote a larger family of distance metrics.
            It is closely related to pairwise string alignments.
        */
        static int levenshtein(const std::string& s0, const std::string& s1)
        {
            // easy cases
            if(s0 == s1)
            {
                return 0;
            }
            if(s0.size() == 0)
            {
                return s1.size();
            }
            if(s1.size() == 0)
            {
                return s0.size();
            }

            std::vector<int> rowA;
            std::vector<int> rowB;

            // init rowA
            for(int i=0; i<=s1.size(); i++)
            {
                rowA.push_back(i);
            }

            // main loop
            for(int i=0; i<s0.size(); i++)
            {
                rowB.push_back(i+1);
                for(int j=0; j<s1.size(); j++)
                {
                    auto cost = ((s0[i] == s1[j]) ? 0 : 1);
                    rowB.push_back(std::min(std::min(rowB[j]+1,rowA[j+1]+1), rowA[j]+cost));
                }
                rowA.clear();
                for(auto d : rowB)
                {
                    rowA.push_back(d);
                }
                rowB.clear();
            }
            // return
            return rowA[rowA.size()-1];
        }
    }
}

#endif // UTIL_LEVENSHTEIN_HPP