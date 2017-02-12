#include "util/ngrams.hpp"

#include <iostream>
#include <string>

using namespace util::string;
using namespace std;

int main()
{
    string text = "loremL";

    for(auto &pair : ngrams(text, 2, true, false))
    {
        cout << pair.first << "\t" << pair.second << endl;
    }

}