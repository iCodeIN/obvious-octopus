#include <iostream>
#include <string>

#include "util/ngrams.hpp"

using namespace util;
using namespace std;

int main()
{
    string text = "loremL";

    for(auto &pair : NGrams::ngrams(text, 2, true, false))
    {
        cout << pair.first << "\t" << pair.second << endl;
    }

}