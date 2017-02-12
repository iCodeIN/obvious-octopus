#ifndef NLP_METAHEURISTICTOKENIZER_HPP
#define NLP_METAHEURISTICTOKENIZER_HPP

#include "nlp/itokenizer.h"

#include <memory>

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

                // set up metric
                auto tokenizationMetric = [s, initialTokenization](std::vector<double>& tuple)
                {
                    auto start = initialTokenization[0];
                    auto end = initialTokenization[initialTokenization.size() - 1];

                    // build tokenization
                    int j = 0;
                    std::string token = "";
                    std::vector<std::string> tokens;
                    for(int i=start; i<=end; i++)
                    {
                        if(i == initialTokenization[j])
                        {
                            if(tuple[j])
                            {
                                if(token.size() != 0)
                                {
                                    tokens.push_back(token);
                                }
                                token = "";
                            }
                            j++;
                        }
                        token += s[i];
                    }

                    // debug
                    for(int i=0; i<tokens.size(); i++)
                    {
                        std::cout << tokens[i] << std::endl;
                    }

                    // each token that constitutes a recognized word gets full marks (proportional to its length)
                    auto recognizedTokenScore = 0.0;

                    // each token that consitutes a pseudo-recognized word gets half marks (proportional to its length)
                    auto pseudoRecognizedTokenScore = 0.0;

                    // each token that occurs more than twice is counted as a fully recognized token
                    auto unrecognizedTokensScore = 0.0;
                    std::map<std::string, int> freq;
                    for(int i=0; i<tokens.size(); i++)
                    {
                        freq[tokens[i]]++;
                    }
                    for(auto &entry : freq)
                    {
                        if(entry.second >= 2)
                        {
                            unrecognizedTokensScore += entry.first.length();
                        }
                    }

                    // minimize nr of tokens
                    auto nofTokensScore = 0.0;

                    // return
                    return recognizedTokenScore + pseudoRecognizedTokenScore + unrecognizedTokensScore + nofTokensScore;
                };

                // debug
                std::cout << "==================================" << std::endl;
                std::vector<double> init;
                for(int i=0; i<initialTokenization.size(); i++)
                {
                    init.push_back(1);
                }

                tokenizationMetric(init);

                // release the meta-heuristic

                // convert result back to std::vector<int>

                // return
            }

            // --- members ---
            std::unique_ptr<ITokenizer> m_tokenizer;
            int                         m_maximumNofCharsPerRange = 64;
    };
}

#endif // NLP_METAHEURISTICTOKENIZER_HPP