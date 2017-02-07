#ifndef METAHEURISTICTOKENIZER_HPP
#define METAHEURISTICTOKENIZER_HPP

#include <memory>

#include "nlp/itokenizer.h"

namespace nlp
{
    /*! ITokenizer implementation using meta-heuristic to improve the result obtained by a more naive ITokenizer
     */
    class MetaheuristicTokenizer : public ITokenizer
    {
        public:
            /*! constructor
             */
            explicit MetaheuristicTokenizer(std::unique_ptr<ITokenizer> tokenizer)
                : m_tokenizer(std::move(tokenizer))
            {
            }

            /*! default destructor
             */
            virtual ~MetaheuristicTokenizer() = default;

            //! --- ITokenizer ---
            std::vector<int> tokenize(const std::string& s) const override
            {
                // perform initial tokenization
                auto initialTokenization = m_tokenizer->tokenize(s);

                // determine workable ranges (to avoid large dimensionality)
                std::vector<int> ranges;
                ranges.push_back(0);
                for(int i=0; i<initialTokenization.size(); i++)
                {
                    auto len = initialTokenization[i] - ranges[ranges.size() - 1];
                    if(len > m_maximumNofCharsPerRange)
                    {
                        ranges.push_back(initialTokenization[i]);
                    }
                }
                if(ranges[ranges.size() - 1] != initialTokenization[initialTokenization.size() - 1])
                {
                    ranges.push_back(initialTokenization[initialTokenization.size() - 1]);
                }

                // delegate to ranges
                for(int i=1; i < ranges.size(); i++)
                {
                    // appropriate subvector
                    std::vector<int> subInitTokenization;
                    for(auto &p : initialTokenization)
                    {
                        if(p >= ranges[i-1] && p<=ranges[i])
                        {
                            subInitTokenization.push_back(p);
                        }
                    }
                    // delegate
                    tokenizeRange(s, subInitTokenization);
                }
            }

        private:
            // --- methods ---
            void tokenizeRange(const std::string& s, std::vector<int>& initialTokenization) const
            {
                auto start = initialTokenization[0];
                auto end = initialTokenization[initialTokenization.size() - 1];

                // set up metric
                auto tokenizationMetric = []()
                {
                    // each token that constitutes a recognized word gets full marks (proportional to its length)
                    // each token that consitutes a pseudo-recognized word gets half marks (proportional to its length)
                    // each token that occurs more than twice is counted as a fully recognized token
                    // minimize nr of tokens
                };

                // release the meta-heuristic

                // convert result back to std::vector<int>

                // return
            }

            // --- members ---
            std::unique_ptr<ITokenizer> m_tokenizer;
            int                         m_maximumNofCharsPerRange = 64;
    };
}

#endif // METAHEURISTICTOKENIZER_HPP