#ifndef OS_HPP
#define OS_HPP

#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace os
{
#ifdef __linux__
    /*! \return true iff the current operating system is a windows variant
     */
    static bool isWindows()
    {
        return false;
    }
    /*! \return true iff the current operating system is a linux variant
     */
    static bool isLinux()
    {
        return true;
    }
#elif _WIN32
    /*! \return true iff the current operating system is a windows variant
     */
    static bool isWindows()
    {
        return true;
    }
    /*! \return true iff the current operating system is a linux variant
     */
    static bool isLinux()
    {
        return false;
    }
#else
    /*! \return true iff the current operating system is a windows variant
     */
    static bool isWindows()
    {
        return false;
    }
    /*! \return true iff the current operating system is a linux variant
     */
    static bool isLinux()
    {
        return false;
    }
#endif

    /*! \return the result of executing a system command
     */
    std::vector<std::string> exec(const std::string& cmd)
    {
        std::array<char, 128> buffer;
        std::vector<std::string> result;
        std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
        if(!pipe)
        {
            throw std::runtime_error("popen() failed!");
        }
        while(!feof(pipe.get()))
        {
            if(fgets(buffer.data(), 128, pipe.get()) != 0)
            {
                // trim trailing newline
                auto temp = std::string(buffer.data());
                if(temp[temp.length() - 1] == '\n')
                {
                    temp = temp.substr(0, temp.length() - 1);
                }
                result.push_back(temp);
            }
        }
        return result;
    }
}

#endif // OS_HPP