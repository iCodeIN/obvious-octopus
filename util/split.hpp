#ifndef SPLIT_HPP
#define SPLIT_HPP

#include <functional>
#include <cctype>
#include <string>
#include <vector>

namespace util
{
    /*! Utility class for splitting std::string
     */
    class Split
    {
        public:
            /*! Split an std::string using an std::function to decide whether to split on a given character
             */
            static std::vector<std::string> split(const std::string& s, std::function<bool(char)> splitFunction)
            {
                std::string temp;
                std::vector<std::string> tokens;
                for(int i=0; i<s.length(); i++)
                {
                    if(splitFunction(s[i]))
                    {
                        tokens.push_back(temp);
                        tokens.push_back(s[i] + "");
                        temp = "";
                    }
                    else
                    {
                        temp += s[i];
                    }
                }
                return tokens;
            }
            /*!  Split an std::string on punctuation characters and whitespace characters
             *   as defined by the ispunct and isspace functions.
             */
            static std::vector<std::string> splitPunctAndSpace(const std::string& s )
            {
                auto f = [](char c)
                {
                    return ispunct(c) || isspace(c);
                };
                return split(s, f);
            }
    };
}

#endif // SPLIT_HPP