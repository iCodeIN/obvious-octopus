#ifndef UTIL_MORSE_HPP
#define UTIL_MORSE_HPP

#include <string>

namespace util
{
    namespace string
    {
        static std::string toMorse(const std::string& s)
        {
            std::string retval = "";
            for(int i=0; i<s.length(); i++)
            {
                char c = std::toupper(s[i]);
                switch(c)
                {
                    case 'A' :
                        retval += ".-";
                        break;
                    case 'B' :
                        retval += "-...";
                        break;
                    case 'C' :
                        retval += "-.-.";
                        break;
                    case 'D' :
                        retval += "-..";
                        break;
                    case 'E' :
                        retval += ".";
                        break;
                    case 'F' :
                        retval += "..-.";
                        break;
                    case 'G' :
                        retval += "--.";
                        break;
                    case 'H' :
                        retval += "....";
                        break;
                    case 'I' :
                        retval += "..";
                        break;
                    case 'J' :
                        retval += ".---";
                        break;
                    case 'K' :
                        retval += "-.-";
                        break;
                    case 'L' :
                        retval += ".-..";
                        break;
                    case 'M' :
                        retval += "--";
                        break;
                    case 'N' :
                        retval += "-.";
                        break;
                    case 'O' :
                        retval += "---";
                        break;
                    case 'P' :
                        retval += ".--.";
                        break;
                    case 'Q' :
                        retval += "--.-";
                        break;
                    case 'R' :
                        retval += ".-.";
                        break;
                    case 'S' :
                        retval += "...";
                        break;
                    case 'T' :
                        retval += "-";
                        break;
                    case 'U' :
                        retval += "..-";
                        break;
                    case 'V' :
                        retval += "...-";
                        break;
                    case 'W' :
                        retval += ".--";
                        break;
                    case 'X' :
                        retval += "-..-";
                        break;
                    case 'Y' :
                        retval += "-.--";
                        break;
                    case 'Z' :
                        retval += "--..";
                        break;
                    case ' ' :
                        retval += "/";
                        break;
                }
                retval += " ";
            }
            return retval;
        }
    }
}

#endif // UTIL_MORSE_HPP