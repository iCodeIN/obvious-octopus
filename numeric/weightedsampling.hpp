#ifndef WEIGHTEDSAMPLING_HPP
#define WEIGHTEDSAMPLING_HPP

#include <algorithm>
#include <assert.h>
#include <map>
#include <stdlib.h>
#include <time.h>
#include <tuple>
#include <vector>

#include <boost/bimap.hpp>

namespace numeric
{
    template <typename T>
    class WeightedSampling final
    {
        public:
            explicit WeightedSampling(const std::map<T, double>& distribution)
            {
                // build a dictionary based on the distribution
                buildDictionary(distribution);

                // build lookup table
                std::map<long,double> keyDistribution;
                for(auto &pair : distribution)
                {
                    auto keyIt = m_dictionary.left.find(pair.first);
                    assert(keyIt != m_dictionary.left.end());
                    auto key = keyIt->second;
                    keyDistribution[key] = pair.second;
                }
                buildLookupTable(keyDistribution);

                // seed random
                srand(time(0));
            }
            explicit WeightedSampling(const boost::bimap<T, long> &dictionary, const std::map<long, double>& distribution)
            {
                // copy the dictionary
                m_dictionary = dictionary;

                // check the given dictionary
                assert(checkDictionary());

                // build lookup table
                buildLookupTable(distribution);

                // seed random
                srand(time(0));
            }

            virtual ~WeightedSampling() = default;

            /*! Prohibit copy constructor
             */
            WeightedSampling(WeightedSampling &n) = delete;

            /*! Prohibit const copy constructor
             */
            WeightedSampling(const WeightedSampling &n) = delete;

            /*! Prohibit assignment operator
             */
            WeightedSampling operator=(WeightedSampling &&) = delete;

            /*! Prohibit const assignment operator
             */
            WeightedSampling operator=(const WeightedSampling &&) = delete;

            /*! \return an item picked from the distribution, with the likelihood provided by the distribution
             */
            const T pick() const
            {
                auto index = rand() % m_lookupTableSize;     // generate an index used for accessing the lookupTable
                auto key = m_lookupTable[index];             // lookup the key at the given index in the lookupTable
                auto valIt = m_dictionary.right.find(key);   // lookup the item corresponding to the key in the dictionary
                assert(valIt != m_dictionary.right.end());   // assert that the item exists
                return valIt->second;                        // return
            }

        private:
            // --- methods ---
            void buildDictionary(const std::map<T,double>& distribution)
            {
                using relation = typename boost::bimap<T,long>::value_type;
                for(auto &pair : distribution)
                {
                    m_dictionary.insert(relation(pair.first, m_dictionary.left.size()));
                }
            }
            bool checkDictionary() const
            {
                // #TODO
                return true;
            }
            void buildLookupTable(const std::map<long, double>& distribution)
            {
                // calculate total probability mass
                auto totalMass = 0.0;
                for(auto &pair : distribution)
                {
                    totalMass += pair.second;
                }
                assert(totalMass > 0);

                // init m_lookupTable
                assert(m_lookupTable.empty());
                assert(m_lookupTableSize > 0);
                m_lookupTable.reserve(m_lookupTableSize);

                // (reverse) sort distribution by probability
                std::vector<std::tuple<long,double>> sortedDistribution;
                for(auto &pair : distribution)
                {
                    auto key = pair.first;
                    auto val = pair.second / totalMass;
                    sortedDistribution.push_back(std::make_tuple(key,val));
                }
                auto cmp = [](const std::tuple<long, double>& t0, const std::tuple<long, double>& t1)
                {
                    return std::get<1>(t0) > std::get<1>(t1);
                };
                std::sort(sortedDistribution.begin(), sortedDistribution.end(), cmp);

                // fill m_lookupTable
                int k = 0;
                double p = std::get<1>(sortedDistribution[k]);
                double r = 0;
                for(int i=0; i<m_lookupTableSize; i++)
                {
                    r = (i + 0.0) / (m_lookupTableSize  + 0.0);
                    m_lookupTable[i] = k;
                    if(r >= p)
                    {
                        k++;
                        p+=std::get<1>(sortedDistribution[k]);
                    }
                }
            }

            // --- members ---
            boost::bimap<T, long>               m_dictionary;
            const static int                    m_lookupTableSize = 100000;
            std::vector<long>                   m_lookupTable;
    };
}

#endif // WEIGHTEDSAMPLING_HPP
