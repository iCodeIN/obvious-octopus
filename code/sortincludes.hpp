#ifndef CODE_SORTINCLUDES_HPP
#define CODE_SORTINCLUDES_HPP

#include <istream>

namespace code
{
    namespace algorithm
    {
        void sortIncludes(const std::string& inputFile)
        {
            // read entire file
            std::vector<std::string> lines;
            ifstream file(inputFile);
            if(file.is_open())
            {
                std::string line;
                while(std::getline(file, line))
                {
                    lines.push_back(line);
                }
            }
            file.close();

            // mark includes

            // store entire file (again)

            // return
        }
    }
}

#endif // CODE_SORTINCLUDES_HPP