#ifndef FILETOSTRING_HPP
#define FILETOSTRING_HPP

#include <fstream>
#include <string>

namespace util
{
    /*! Utility class for reading entire files and streams to an std::string
     */
    class FileToString
    {
        public:
            /*! Prohibit construction of FileToString. This class offers only static methods.
             */
            explicit FileToString() = delete;

            /*! \return the result of consuming the entire std::istream as an std::string
             */
            static std::string readStream(std::istream& s)
            {
                std::string temp = "";
                std::string retval = "";
                while(getline(s, temp))
                {
                    retval += temp;
                }
                return retval;
            }

            /*! \return the result of the consuming the entire file as an std::string
             */
            static std::string readFile(std::string filename)
            {
                std::ifstream fileStream(filename);
                if(fileStream.is_open())
                {
                    auto retval = readStream(fileStream);
                    fileStream.close();
                    return retval;
                }
                else
                {
                    return "";
                }
            }

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // FILETOSTRING_HPP