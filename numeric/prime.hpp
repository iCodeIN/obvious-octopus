#pragma once
#ifndef PRIME_HPP
#define PRIME_HPP

#include <assert.h>
#include <random>
#include <tuple>
#include <vector>

namespace numeric
{
    /*! A prime number (or a prime) is a natural number greater than 1 that has no positive divisors other than 1 and itself. A natural number greater than 1 that is not a prime number is called a composite number. For example, 5 is prime because 1 and 5 are its only positive integer factors,           whereas 6 is composite because it has the divisors 2 and 3 in addition to 1 and 6. The fundamental theorem of arithmetic establishes the central role of primes in number theory: any integer greater than 1 can be expressed as a product of primes that is unique up to ordering. The uniqueness      in this theorem requires excluding 1 as a prime because one can include arbitrarily many instances of 1 in any factorization, e.g., 3, 1 · 3, 1 · 1 · 3, etc. are all valid factorizations of 3.
     */
    class Prime
    {
        public:

            /*! In number theory, a probable prime (PRP) is an integer that satisfies a specific condition that is satisfied by all prime numbers, but which is not satisfied by most composite numbers. Different types of probable primes have different specific conditions. While there may be probable     primes that are composite (called pseudoprimes), the condition is generally chosen in order to make such exceptions rare.
                Fermat's test for compositeness, which is based on Fermat's little theorem, works as follows: given an integer n, choose some integer a coprime to n and calculate an − 1 modulo n. If the result is different from 1, then n is composite. If the result is 1, then n is likely to be prime; n is then called a probable prime to base a. A weak probable prime to base a is an integer that is a probable prime to base a, but which is not a strong probable prime to base a (see below).
             */
            static bool isProbablePrime(ulong n)
            {
                std::vector<ulong> w;
                // if n < 1373653, it is enough to test a = 2 and 3
                if(n < 1373653L)
                {
                    w.push_back(2L);
                    w.push_back(3L);
                    return isProbablePrime(n, w);
                }
                // if n < 9080191, it is enough to test a = 31 and 73
                if(n < 9080191L)
                {
                    w.clear();
                    w.push_back(31L);
                    w.push_back(73L);
                    return isProbablePrime(n, w);
                }
                // if n < 4759123141, it is enough to test a = 2, 7, and 61
                if(n < 4759123141L)
                {
                    w.clear();
                    w.push_back(2L);
                    w.push_back(7L);
                    w.push_back(61L);
                    return isProbablePrime(n, w);
                }
                // if n < 1122004669633, it is enough to test a = 2, 13, 23, and 1662803
                if(n < 1122004669633L)
                {
                    w.clear();
                    w.push_back(2L);
                    w.push_back(13L);
                    w.push_back(23L);
                    w.push_back(1662803L);
                    return isProbablePrime(n, w);
                }
                // if n < 2152302898747, it is enough to test a = 2, 3, 5, 7, and 11
                if(n < 2152302898747L)
                {
                    w.clear();
                    w.push_back(2L);
                    w.push_back(3L);
                    w.push_back(5L);
                    w.push_back(7L);
                    w.push_back(11L);
                    return isProbablePrime(n, w);
                }
                // if n < 3474749660383, it is enough to test a = 2, 3, 5, 7, 11, and 13
                if(n < 3474749660383L)
                {
                    w.clear();
                    w.push_back(2L);
                    w.push_back(3L);
                    w.push_back(5L);
                    w.push_back(7L);
                    w.push_back(11L);
                    w.push_back(13L);
                    return isProbablePrime(n, w);
                }
                // if n < 341550071728321, it is enough to test a = 2, 3, 5, 7, 11, 13, and 17
                if(n < 341550071728321L)
                {
                    w.clear();
                    w.push_back(2L);
                    w.push_back(3L);
                    w.push_back(5L);
                    w.push_back(7L);
                    w.push_back(11L);
                    w.push_back(13L);
                    w.push_back(17L);
                    return isProbablePrime(n, w);
                }
                // general case
                w.clear();
                return isProbablePrime(n, w);
            }

            /*! For any given number, find the next prime number in the sequence of primes.
             */
            static ulong nextPrime(ulong p)
            {
                p++;
                while(!isProbablePrime(p))
                {
                    p++;
                }
                return p;
            }

            /*! Prime factorization or integer factorization of a number is the determination of the set of prime numbers which multiply together to give the original integer.
                It is also known as prime decomposition.
             */
            static std::vector<ulong> primeFactorization(ulong l)
            {
                std::vector<ulong> divs;
                auto p = 2L;
                while(l != 1)
                {
                    while(l%p==0)
                    {
                        l/=p;
                        divs.push_back(p);
                    }
                    p = nextPrime(p);
                }
                return divs;
            }

            /*! Prime factorization or integer factorization of a number is the determination of the set of prime numbers which multiply together to give the original integer.
                It is also known as prime decomposition.
                \returns a vector of primes and their corresponding power, that make up the prime factorization.
             */
            static std::vector<std::tuple<ulong,ulong>> primeFactorization2(ulong l)
            {
                // asserts
                assert(l > 1);

                std::vector<std::tuple<ulong,ulong>> retval;
                auto prime = 2L;
                auto power = 0L;
                while(l != 1L)
                {
                    while(l % prime == 0L)
                    {
                        l /= prime;
                        power++;
                    }
                    if(power != 0L)
                    {
                        retval.push_back(std::make_tuple(prime, power));
                    }
                    // next prime
                    prime = nextPrime(prime);
                    power = 0L;
                }
                return retval;
            }

            /*! In number theory, Euler's totient function counts the positive integers up to a given integer n that are relatively prime to n.
                It is written using the Greek letter phi as φ(n) or ϕ(n), and may also be called Euler's phi function.
                It can be defined more formally as the number of integers k in the range 1 ≤ k ≤ n for which the greatest common divisor gcd(n, k) = 1;
                The integers k of this form are sometimes referred to as totatives of n.
             */
            static ulong totient(ulong l)
            {
                auto tot = 1L;
                for(auto &tuple :  primeFactorization2(l))
                {
                    if(std::get<1>(tuple) == 1L)
                    {
                        tot *= (std::get<0>(tuple) - 1);
                    }
                    else
                    {
                        tot *= pow(std::get<0>(tuple), std::get<1>(tuple) - 1);
                    }
                }
                return tot;
            }

        private:

            /*! \return base ^ exp % mod
             */
            static ulong modpow(ulong base, ulong exp, ulong mod)
            {
                // assert
                assert(!(base==0 && exp==0));

                // exceptions
                if(exp == 0)
                {
                    return 1;
                }
                if(exp == 1)
                {
                    return base;
                }

                // main loop
                ulong retval = 1;
                while(exp != 1)
                {
                    if(exp % 2 == 0)
                    {
                        base = ((base % mod) * (base % mod)) % mod;
                        exp /= 2;
                    }
                    else
                    {
                        retval = ((retval % mod) * (base % mod)) % mod;
                        exp -= 1;
                    }
                }
                return (retval * base) % mod;
            }

            /*! miller rabin primality test, with given witnesses
             */
            static bool isProbablePrime(ulong n, std::vector<ulong> w)
            {
                // easy
                if(n==2 || n==3 || n==5 || n==7)
                {
                    return true;
                }
                if(n<10)
                {
                    return false;
                }

                // write (n-1) as 2 ^ s * d
                auto d = n - 1L;
                auto s = 0L;
                while(d%2==0)
                {
                    d/=2;
                    s++;
                }

                // witness loop
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<ulong> dis(1, n - 1);
                bool nextWitness = false;
                for(int k=0; k<(w.empty() ? 10 : w.size()); k++)
                {
                    // random base between 1 and n - 1
                    auto a = w.empty() ? dis(gen) : w[k];

                    // mod pow
                    auto x = modpow(a, d, n);

                    if(x == 1 || x == n - 1)
                    {
                        continue;
                    }

                    // modular exponentiation with repeated squaring
                    for(auto i=s-1; i>=0; i--)
                    {
                        x = (x * x) % n;

                        // composite
                        if(x == 1)
                        {
                            return false;
                        }

                        if(x==n-1)
                        {
                            // the behaviour of this flag, and the break are meant to emulate a 'continue <loopname>' statement
                            nextWitness = true;
                            break;
                        }
                    }
                    if(!nextWitness)
                    {
                        return false;
                    }
                    nextWitness = false;
                }

                // probably prime
                return true;
            }
    };
}

#endif // PRIME_HPP
