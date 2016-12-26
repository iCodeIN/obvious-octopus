#ifndef BIGINTEGER_HPP
#define BIGINTEGER_HPP

#include <assert.h>
#include <iostream> // debug
#include <string>
#include <vector>

namespace numeric
{
    /*! Immutable arbitrary-precision integers. All operations behave as if BigIntegers were represented in base 10 notation. BigInteger provides analogues to all of C++'s primitive integer operators,
        and all other relevant methods. Additionally, BigInteger provides operations for modular arithmetic, GCD calculation, primality testing, prime generation, bit manipulation,
        and a few other miscellaneous operations.
     */
    class BigInteger
    {
        public:
            /*! string based constructor
             */
            BigInteger(std::string s)
            {

            }

            /*! int-based constructor
             */
            BigInteger(int i)
            {
                fromLong(i);
            }

            /*! long-based constructor
             */
            BigInteger(long l)
            {
                fromLong(l);
            }

            /*! copy constructor
             */
            BigInteger(const BigInteger& i0)
            {
                for(int i=0; i<i0.m_digits.size(); i++)
                {
                    m_digits.push_back(i0.m_digits[i]);
                }
                m_negative = i0.m_negative;
            }

            /*! default destructor
             */
            virtual ~BigInteger() = default;

            /*! \return i0 + i1
             */
            static BigInteger add(const BigInteger& i0, const BigInteger& i1)
            {
                // redirects
                if(i0.m_negative && !i1.m_negative)
                {
                    return sub(i1, i0);
                }
                if(i1.m_negative && !i0.m_negative)
                {
                    return sub(i0, i1);
                }

                // init retval
                BigInteger retval = BigInteger(0);
                retval.m_digits.clear();

                // main loop
                auto maxNofDigits = std::max(i0.m_digits.size(), i1.m_digits.size());
                auto carry = 0;
                for(int i=0; i<maxNofDigits; i++)
                {
                    auto d0 = i >= i0.m_digits.size() ? 0 : i0.m_digits[i];
                    auto d1 = i >= i1.m_digits.size() ? 0 : i1.m_digits[i];
                    auto d = d0 + d1 + carry;
                    carry = 0;
                    if(d >= 10)
                    {
                        carry = d / 10;
                        d = d % 10;
                    }
                    retval.m_digits.push_back(d);
                }

                // final carry
                if(carry > 0)
                {
                    retval.m_digits.push_back(carry);
                }

                // set sign
                retval.m_negative = i0.m_negative && i1.m_negative;

                // return
                return retval;
            }

            /*! \return i0 - i1
             */
            static BigInteger sub(const BigInteger& i0, const BigInteger& i1)
            {
                // redirects
                if(i0.m_negative && !i1.m_negative)
                {
                    auto temp = BigInteger(i1);
                    temp.m_negative = true;
                    return add(i0, temp);
                }
                if(!i0.m_negative && i1.m_negative)
                {
                    auto temp = BigInteger(i1);
                    temp.m_negative = false;
                    return add(i0, temp);
                }

                // compare numbers
                bool negative = false;
                const BigInteger* arg0 = &i0;
                const BigInteger* arg1 = &i1;
                auto cmp = compare(i0, i1);
                if(cmp == 0)
                {
                    return BigInteger(0);
                }
                else if(cmp > 0)
                {
                    // '-100' - '-1000'
                    if(i0.m_negative)
                    {
                        // swap numbers
                        arg0 = &i1;
                        arg1 = &i0;
                    }
                    // '1000' - '100'
                    else
                    {

                    }
                }
                else
                {
                    // '-1000' - '-100'
                    if(i0.m_negative)
                    {

                    }
                    // '100' - '1000'
                    else
                    {
                        // swap numbers
                        arg0 = &i1;
                        arg1 = &i0;
                    }
                }

                // init retval
                BigInteger retval = BigInteger(0);
                retval.m_digits.clear();
                std::cout << retval.toString() << std::endl;

                // main loop
                auto maxNofDigits = std::max(i0.m_digits.size(), i1.m_digits.size());
                auto borrow = 0;
                for(int i=0; i<maxNofDigits; i++)
                {
                    auto d0 = i >= arg0->m_digits.size() ? 0 : arg0->m_digits[i];
                    auto d1 = i >= arg1->m_digits.size() ? 0 : arg1->m_digits[i];
                    auto d = d0 - borrow - d1;
                    borrow = 0;
                    if(d < 0)
                    {
                        borrow = 1;
                        d = d + 10;
                    }
                    retval.m_digits.push_back(d);
                }
                assert(borrow == 0);

                // truncate
                while(retval.m_digits[retval.m_digits.size()-1] == 0)
                {
                    retval.m_digits.erase(retval.m_digits.begin() + retval.m_digits.size() -1);
                }

                // sign
                retval.m_negative = (cmp < 0);

                // return
                return retval;
            }

            /*! \return i0 * i1
             */
            static BigInteger mul(const BigInteger& i0, const BigInteger& i1)
            {
                // init retval
                BigInteger retval = BigInteger(0);

                // main loop
                BigInteger temp = BigInteger(0);
                temp.m_digits.clear();

                for(int i=0; i<i0.m_digits.size(); i++)
                {
                    temp.m_digits.clear();
                    for(int j=0; j<i; j++)
                    {
                        temp.m_digits.push_back(0);
                    }

                    auto d0 = i0.m_digits[i];
                    auto carry = 0;
                    for(int j=0; j<i1.m_digits.size(); j++)
                    {
                        auto d1 = i1.m_digits[j];
                        auto d = d0 * d1 + carry;
                        carry = 0;
                        if(d >= 10)
                        {
                            carry = d / 10;
                            d = d % 10;
                        }
                        temp.m_digits.push_back(d);
                    }

                    while(carry != 0)
                    {
                        temp.m_digits.push_back(carry % 10);
                        carry /= 10;
                    }

                    retval = add(retval, temp);
                }

                // truncate
                while(retval.m_digits[retval.m_digits.size()-1] == 0)
                {
                    retval.m_digits.erase(retval.m_digits.begin() + retval.m_digits.size() -1);
                }

                // sign
                retval.m_negative = (i0.m_negative || i1.m_negative) && !(i0.m_negative && i1.m_negative);

                return retval;
            }

            /*! \return |i|
             */
            static BigInteger abs(const BigInteger& i)
            {
                auto retval = BigInteger(i);
                if(retval.m_negative)
                {
                    retval.m_negative = false;
                }
                return retval;
            }

            /*! \return i0 mod i1
             */
            static BigInteger mod(const BigInteger& i0, const BigInteger& i1)
            {

            }

            /*! \return i0 ^ i1
             */
            static BigInteger pow(const BigInteger& i0, const BigInteger& i1)
            {

            }

            /*! \return -1 iff i0 < i1, 0 iff i0 == i1, 1 iff i0 > i1
             */
            static int compare(const BigInteger& i0, const BigInteger& i1)
            {
                // 1. if the numbers have a different sign, this is easy
                if(i0.m_negative && !i1.m_negative)
                {
                    return -1;
                }
                if(!i0.m_negative && i1.m_negative)
                {
                    return 1;
                }

                // 2. both numbers have the same sign, but different radix (number of digits)
                if(i0.m_digits.size() > i1.m_digits.size())
                {
                    // '999' > '9' and '-999' < '-9'
                    return i0.m_negative ? -1 : 1;
                }
                if(i0.m_digits.size() < i1.m_digits.size())
                {
                    // '9' < '999' and '-9' < '-999'
                    return i0.m_negative ? 1 : -1;
                }

                // 3. both numbers have the same sign and same radix
                for(int i=i0.m_digits.size()-1; i>=0; i--)
                {
                    auto d0 = i0.m_digits[i];
                    auto d1 = i1.m_digits[i];
                    // '99_8_66' > '99_5_35'
                    if(d0 > d1)
                    {
                        return i0.m_negative ? -1 : 1;
                    }
                    // '99_5_35' < '99_8_66'
                    if(d0 < d1)
                    {
                        return i0.m_negative ? 1 : -1;
                    }
                }

                // 4. default
                return 0;
            }

            std::string toString()
            {
                // special case for 0
                if(m_digits.size() == 0 || m_digits.size() == 1 && m_digits[0] == 0)
                {
                    return "0";
                }

                // general case
                std::string s = "";
                for(int i=0; i<m_digits.size(); i++)
                {
                    s = std::to_string(m_digits[i]) + s;
                }
                if(m_negative)
                {
                    s  = "-" + s;
                }
                return s;
            }

        private:
            // --- methods ---
            void fromLong(long l)
            {
                // handle 0
                if(l == 0 || l == -0)
                {
                    m_digits.push_back(0);
                    m_negative = false;
                    return;
                }
                // handle sign
                if(l < 0)
                {
                    m_negative = true;
                    l = -l;
                }
                else
                {
                    m_negative = false;
                }
                // process digits
                while(l != 0)
                {
                    auto d = l % 10;
                    l /= 10;
                    m_digits.push_back(d);
                }
            }

            void fromString(const std::string& s)
            {
            }

            // --- members ---
            std::vector<short>  m_digits;
            bool                m_negative;
    };
}

#endif // BIGINTEGER_HPP