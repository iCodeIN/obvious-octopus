#include "os/file.hpp"
#include "util/stringutils.hpp"

#include <fstream>
#include <iostream>
#include <vector>

using namespace os;

void process(const File& f)
{
    std::vector<std::string> lines;
    std::ifstream fileStream(f.getPath());
    if(fileStream.is_open())
    {
        std::string temp = "";
        while(getline(fileStream, temp))
        {
            lines.push_back(temp);
        }
    }
    fileStream.close();

    std::cout << "read " << lines.size() << " lines." << std::endl;

}

void exploreDirectory(const File& f)
{
    auto name = f.getName();
    if(util::string::endsWith(name, ".cpp"))
    {
        std::cout << "CPP file found : " << name << std::endl;
        process(f);
    }
    else if(util::string::endsWith(name, ".hpp"))
    {
        std::cout << "HPP file found : " << name << std::endl;
        process(f);
    }
    else if(util::string::endsWith(name, ".h"))
    {
        std::cout << "H file found : " << name << std::endl;
        process(f);
    }
    if(f.isDirectory())
    {
        for(auto &child : f.list())
        {
            exploreDirectory(child);
        }
    }
}

int main()
{
    auto root = File("/home/joris/Code/alexandria");

    // explore directory
    exploreDirectory(root);

    // return
    return 0;
}