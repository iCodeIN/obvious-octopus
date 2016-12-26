#include "util/split.hpp"

#include <fstream>
#include <iostream>
#include <string>

using namespace util;
using namespace std;

int main()
{
    // read input
    string temp;
    string input;
    while(getline(cin, temp))
    {
        input += temp;
    }

    auto tokens = Split::splitPunctAndSpace(input);
    cout << "read " << tokens.size() << " tokens." << endl;

    return 0;
}
