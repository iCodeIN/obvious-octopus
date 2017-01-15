#include <clocale>
#include <cwchar>
#include <iostream>
#include <set>
#include <string>
#include <fstream>

// #include "util/filetostring.hpp"

using namespace std;
//using namespace util;

static std::wstring readStream(std::wistream& s)
{
    std::cout << "<readStream>" << std::endl;
    std::locale loc("");
    s.imbue(loc);
    std::wstring temp = L"";
    std::wstring retval = L"";
    std::cout << s.good() << std::endl;
    while(getline<wchar_t>(s, temp))
    {
        retval += temp;
    }
    return retval;
}

/*! \return the result of the consuming the entire file as an std::string
 */
static std::wstring readFile(std::string filename)
{
    std::cout << "<readFile>" << std::endl;
    std::wifstream fileStream(filename);
    if(fileStream.is_open())
    {
        auto retval = readStream(fileStream);
        fileStream.close();
        return retval;
    }
    else
    {
        return L"";
    }
}

int main()
{
    std::setlocale(LC_ALL, "en_US.UTF-8"); // the C locale will be the UTF-8 enabled English
    auto text = readFile("/home/joris/Documents/corpus/de/13953-0.txt");
    //auto text = wstring(L"özekfep213456");

    set<wchar_t> chrs;
    wstring s0 = L"";
    wstring s1 = L"";
    wstring s2 = L"";
    std::cout << text.size() << std::endl;
    for(int i=0; i<text.size(); i++)
    {
        auto c = text[i];
        if(chrs.find(c) != chrs.end())
        {
            continue;
        }
        chrs.insert(c);
        if(std::iswalpha(c))
        {
            s0 += c;
        }
        else if(std::iswdigit(c))
        {
            s1 += c;
        }
        else
        {
            s2 += c;
        }
    }
    std::wcout << s0.size() << L"\t" << s0 << std::endl;
    std::wcout << s1.size() << L"\t" << s1 << std::endl;
    std::wcout << s2.size() << L"\t" << s2 << std::endl;
}