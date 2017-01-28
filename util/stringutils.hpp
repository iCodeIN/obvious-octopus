#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <functional>
#include <cctype>
#include <string>
#include <vector>

namespace util
{
    /*! Utility class for handling std::string
     */
    namespace string
    {
        static bool endsWith(const std::string& s, const std::string& suffix)
        {
            if(s.length() < suffix.length())
            {
                return false;
            }
            for(int i=0; i<suffix.length(); i++)
            {
                if(s[s.length() - 1 - i] != suffix[suffix.length() - 1 - i])
                {
                    return false;
                }
            }
            return true;
        }

        /*! \return an std::string where each character of the input has been converted to uppercase by calling std::toupper
         */
        static std::string toUpper(const std::string& s)
        {
            std::string retval;
            for(int i=0; i<s.size(); i++)
            {
                retval += std::toupper(s[i]);
            }
            return retval;
        }

        /*! \return an std::string where each character of the input has been converted to lowercase by calling std::tolower
         */
        static std::string toLower(const std::string& s)
        {
            std::string retval;
            for(int i=0; i<s.size(); i++)
            {
                retval += std::tolower(s[i]);
            }
            return retval;
        }


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
    }
}

#endif // STRINGUTILS_HPP