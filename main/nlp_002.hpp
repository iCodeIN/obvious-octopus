#include <iostream>
#include <string>

#include "util/ngrams.hpp"

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