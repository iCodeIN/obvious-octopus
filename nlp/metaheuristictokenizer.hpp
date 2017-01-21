#ifndef METAHEURISTICTOKENIZER_HPP
#define METAHEURISTICTOKENIZER_HPP

#include <memory>

#include "nlp/itokenizer.hpp"

namespace nlp
{
    class MetaheuristicTokenizer : public ITokenizer
    {
        public:
            /*! constructor
             */
            explicit MetaheuristicTokenizer(std::unique_ptr<ITokenizer> tokenizer)
                : m_tokenizer(tokenizer)
            {
            }

            /*! destructor
             */
            virtual ~MetaheuristicTokenizer() = default;

            // --- ITokenizer ---
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
                for(int i=1; i<ranges.size(); i++)
                {
                    tokenizeRange(s, ranges[i-1], ranges[i]);
                }
            }

        private:
            // --- methods ---
            void tokenizeRange(const std::string& s, int start, int end)
            {

            }

            // --- members ---
            std::unique_ptr<ITokenizer> m_tokenizer;
            int                         m_maximumNofCharsPerRange = 64;
    };
}

#endif // METAHEURISTICTOKENIZER_HPP