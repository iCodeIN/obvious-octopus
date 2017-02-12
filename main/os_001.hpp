#include "os/file.hpp"
#include "os/os.hpp"

#include <string>

using namespace std;
using namespace os;

int main()
{
    auto path = File("/home/joris/Desktop");
    for(auto &file : path.list())
    {
        cout << (file.exists() ? "E" : "_") << "\t" << (file.isDirectory() ? "D" : "F") << "\t" << file.getPath() << endl;
    }
    return 0;
}