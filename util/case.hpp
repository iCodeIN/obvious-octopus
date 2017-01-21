#ifndef CASE_HPP
#define CASE_HPP

#include <string>

namespace util
{
    class Case final
    {
        public:
            /*! Prohibit construction of Split. This class offers only static methods.
             */
            explicit Case() = delete;

            static std::string toUpper(const std::string& s)
            {
                std::string retval;
                for(int i=0; i<s.size(); i++)
                {
                    retval += std::toupper(s[i]);
                }
                return retval;
            }

            static std::string toLower(const std::string& s)
            {
                std::string retval;
                for(int i=0; i<s.size(); i++)
                {
                    retval += std::tolower(s[i]);
                }
                return retval;
            }

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // CASE_HPP