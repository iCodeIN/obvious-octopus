#ifndef FILE_HPP
#define FILE_HPP

#include "os/os.hpp"

#include <stdio.h>
#include <string>
#include <vector>

#ifdef __linux__
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#elif _WIN32
#endif

namespace os
{
    /*! An abstract representation of file and directory pathnames.
     */
    class File final
    {
        public:
            /*! Creates a new File instance by converting the given pathname string into an abstract pathname.
             */
            explicit File(const std::string& s)
            {
                auto fileSeparator = '/';
                std::string token = "";
                for(int i=0; i<s.length(); i++)
                {
                    if(s[i] == fileSeparator)
                    {
                        path.push_back(token);
                        token = "";
                    }
                    else
                    {
                        token += s[i];
                    }
                }
                if(token.length() != 0)
                {
                    path.push_back(token);
                }
            }

            /*! Creates a new File instance from a parent abstract pathname and a child pathname string.
             */
            explicit File(const File& parent, const std::string& file)
            {
                for(auto &part : parent.path)
                {
                    path.push_back(part);
                }
                path.push_back(file);
            }

            /*! \return true iff the application can read the File
             */
            bool canRead() const
            {
#ifdef __linux__
                struct stat tmp;
                if(stat(getPath().c_str(), &tmp) != 0)
                {
                    return false;
                }
#elif _WIN32
                // #TODO
                return false;
#else
                return false;
#endif
            }

            /*! \return true iff the File was successfully deleted
             */
            bool deleteFile() const
            {
                return remove(getPath().c_str()) == 0;
            }

            /*! \return true iff the File exists
             */
            bool exists() const
            {
#ifdef __linux__
                struct stat tmp;
                return (stat(getPath().c_str(), &tmp) == 0);
#elif _WIN32
                //#TODO
                return false;
#else
                return false;
#endif
            }

            /*! \return the name of the file or directory denoted by this abstract pathname.
             */
            std::string getName() const
            {
                return path[path.size()-1];
            }

            /*! \return the abstract pathname of this abstract pathname's parent, or null if this pathname does not name a parent directory.
             */
            File getParentFile() const
            {
                auto f = File("");
                for(int i=0; i<path.size()-1; i++)
                {
                    f.path.push_back(path[i]);
                }
                return f;
            }

            /*! Converts this abstract pathname into a pathname string.
             */
            std::string getPath() const
            {
                std::string retval = "";
                auto fileSeparator = "/";
                for(int i=0; i < path.size(); i++)
                {
                    retval += ( i==0 ? "" : fileSeparator)  + path[i];
                }
                return retval;
            }

            /*! \return true iff the File denoted by this abstract pathname is a directory.
             */
            bool isDirectory() const
            {
#ifdef __linux__
                struct stat tmp;
                if( stat(getPath().c_str(), &tmp) == 0 )
                {
                    if( tmp.st_mode & S_IFDIR )
                    {
                        return true;
                    }
                }
                return false;
#elif _WIN32
                // #TODO
                return false;
#else
                return false;
#endif
            }

            /*! \return an std::vector<File> of abstract pathnames denoting the files in the directory denoted by this abstract pathname.
             */
            std::vector<File> list() const
            {
                auto fullPath = getPath();
                std::vector<File> retval;

#ifdef __linux__
                DIR           *d;
                struct dirent *dir;
                d = opendir(getPath().c_str());
                if (d)
                {
                    while ((dir = readdir(d)) != NULL)
                    {
                        auto name = std::string(dir->d_name);
                        // unix has the unfortunate habit of sneaking in the virtual paths "." and ".."
                        if(name.compare(".")==0 || name.compare("..") == 0)
                        {
                            continue;
                        }
                        retval.push_back(File(*this, name));
                    }
                    closedir(d);
                }
#elif _WIN32
#else
#endif
                return retval;
            }

            /*! \return true iff the directory denoted by this abstract pathname was successfully created
             */
            bool makeDirectory() const
            {
#ifdef __linux__
                mkdir(getPath().c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
                return true;
#elif _WIN32
                // #TODO
                return false;
#else
                return false;
#endif
            }

        private:
            // --- methods ---
            // --- members ---
            std::vector<std::string> path;
    };
}

#endif // FILE_HPP