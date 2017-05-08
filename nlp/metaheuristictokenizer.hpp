#ifndef NLP_METAHEURISTICTOKENIZER_HPP
#define NLP_METAHEURISTICTOKENIZER_HPP

#include "nlp/itokenizer.h"
#include "nlp/dictionary.hpp"
#include "meta/tabusearch.hpp"

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
            explicit MetaheuristicTokenizer(std::unique_ptr<ITokenizer> tokenizer, std::unique_ptr<Dictionary> dictionary)
                : m_tokenizer(std::move(tokenizer))
                , m_dictionary(std::move(dictionary))
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
                // ensure the last boundary is included
                if(ranges[ranges.size() - 1] != initialTokenization[initialTokenization.size() - 1])
                {
                    ranges.push_back(initialTokenization[initialTokenization.size() - 1]);
                }

                // delegate to ranges
                std::vector<int> retval;
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

                    // gather results
                    for(int p : subInitTokenization)
                    {
                        retval.push_back(p);
                    }

                }

                // return
                return retval;
            }

        private:

            /*!
             */
            std::vector<std::string> toTokens(const std::string& s, const std::vector<int>& tokenization, const std::vector<double>& state) const
            {
                auto start = tokenization[0];
                auto end = tokenization[tokenization.size() - 1];

                // build tokenization
                int j = 0;
                std::string token = "";
                std::vector<std::string> tokens;
                for(int i=start; i<=end; i++)
                {
                    if(i == tokenization[j] && state[j])
                    {
                        if(token.size() != 0)
                        {
                            tokens.push_back(token);
                        }
                        token = "";
                        j++;
                    }
                    if(i < s.size())
                    {
                        token += s[i];
                    }
                }
                if(token.size() != 0)
                {
                    tokens.push_back(token);
                }

                return tokens;
            }

            /*!
             */
            double score(const std::string& s, const std::vector<int>& tokenization, const std::vector<double>& state) const
            {
                auto recognizedTokenScore = 0.0;
                auto pseudoRecognizedTokenScore = 0.0;
                auto unrecognizedTokensScore = 0.0;
                auto nofTokens = 0;
                std::map<std::string, int> freq;
                std::cout << "";
                for(auto &token : toTokens(s, tokenization, state))
                {
                    nofTokens++;
                    if(m_dictionary->isWord(token))
                    {
                        recognizedTokenScore += token.size();
                        std::cout << token << " [R]";
                    }
                    else if(token.size() > 2 && m_dictionary->isPseudoWord(token, 1))
                    {
                        pseudoRecognizedTokenScore += (token.size() - 1);
                        std::cout << token << " [P]";
                    }
                    else
                    {
                        freq[token]++;
                        std::cout << token << " [U]";
                    }
                    //std::cout << token << "|";
                }
                for(auto &entry : freq)
                {
                    if(entry.second >= 2)
                    {
                        unrecognizedTokensScore += entry.first.length() * entry.second;
                    }
                }
                auto nofTokensScore =  1.0 - ((nofTokens+0.0) / tokenization.size());
                std::cout << "\t\t" << recognizedTokenScore << "\t" <<  pseudoRecognizedTokenScore << "\t" <<  unrecognizedTokensScore << "\t" << nofTokensScore << std::endl;
                return recognizedTokenScore + unrecognizedTokensScore + pseudoRecognizedTokenScore;
            }

            // --- methods ---
            void tokenizeRange(const std::string& s, std::vector<int>& initialTokenization) const
            {

                // set up metric
                auto tokenizationMetric = [s, initialTokenization, this](const std::vector<double>& tuple)
                {
                    return score(s, initialTokenization, tuple);
                };

                // debug
                std::vector<std::tuple<double,double>> domains;
                std::vector<double> epsilon;
                std::vector<double> seed;
                for(int i=0; i<initialTokenization.size(); i++)
                {
                    seed.push_back(0);
                    epsilon.push_back(1);
                    domains.push_back(std::make_tuple<double,double>(0.0, 1.0));
                }

                // release the meta-heuristic
                auto tabuSearch = meta::TabuSearch();
                auto bestSolution = tabuSearch.findMax(seed, tokenizationMetric, domains, 128, epsilon);

                // convert result back to std::vector<int>
                auto offset = 0;
                for(int i=0; i<bestSolution.size(); i++)
                {
                    std::cout << bestSolution[i] << " ";
                    if(!bestSolution[i])
                    {
                        initialTokenization.erase(initialTokenization.begin() + i - offset);
                        offset++;
                    }
                }
                std::cout << std::endl;

            }

            // --- members ---
            std::unique_ptr<ITokenizer> m_tokenizer;
            std::unique_ptr<Dictionary> m_dictionary;
            int                         m_maximumNofCharsPerRange = 64;
    };
}

#endif // NLP_METAHEURISTICTOKENIZER_HPP