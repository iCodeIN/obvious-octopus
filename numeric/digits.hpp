#ifndef NUMERIC_DIGITS_HPP
#define NUMERIC_DIGITS_HPP

#include <algorithm>
#include <math.h>
#include <vector>

namespace numeric
{
    /*! Utility class for manipulating the digits in a number.
     */
    class Digits
    {
        public:
            /*! \return a vector<int> of the digits of l
                This function does not take into account trailing zeroes.
                eg: digits(001) = {1}
                    digits(123) = {3,2,1}
             */
            static std::vector<int> digits(ulong l)
            {
                std::vector<int> retval;
                if(l == 0L)
                {
                    retval.push_back(0);
                }
                while(l != 0L)
                {
                    retval.push_back(l % 10);
                    l /= 10;
                }
                return retval;
            }

            /*! The digital root (also repeated digital sum) of a non-negative integer is the (single digit) value obtained by an iterative process of summing digits,
                on each iteration using the result from the previous iteration to compute a digit sum. The process continues until a single-digit number is reached.
                For example, the digital root of 65,536 is 7, because 6 + 5 + 5 + 3 + 6 = 25 and 2 + 5 = 7.
             */
            static ulong digitalRoot(ulong l)
            {
                return l - 9L * floor((l-1L)/9L);
            }

            /*! \return true if the digits of l0 can be permuted to generate the sequence of digits of l1
                This method does not keep into account trailing zeroes.
                eg: isDigitPermutation(001,10)  = false
                    isDigitPermutation(123,312) = true
             */
            static bool isDigitPermutation(ulong l0, ulong l1)
            {
                auto d0 = digits(l0);
                auto d1 = digits(l1);
                sort(d0.begin(), d0.end());
                sort(d1.begin(), d1.end());
                return d0 == d1;
            }
    };
}

#endif // NUMERIC_DIGITS_HPP