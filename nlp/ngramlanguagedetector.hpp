#ifndef NGRAMLANGUAGEDETECTOR_HPP
#define NGRAMLANGUAGEDETECTOR_HPP

#include "numeric/distribution.hpp"
#include "util/ngrams.hpp"
#include "xml/xml.hpp"

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace nlp
{
    /*!
     */
    class NGramLanguageDetector
    {
        public:

            /*!
             */
            void train(const std::string& text, const std::string& language)
            {
                // setup
                std::vector<int> lengths;
                lengths.push_back(1);
                lengths.push_back(2);
                lengths.push_back(3);

                // extract ngrams
                auto localModel = util::NGrams::ngrams(text, lengths);

                // add to global model
                for(auto &pair : localModel)
                {
                    m_models[language][pair.first] += pair.second;
                }
            }

            /*!
             */
            std::map<std::string, double> detect(const std::string& text) const
            {
                // setup
                std::vector<int> lengths;
                lengths.push_back(1);
                lengths.push_back(2);
                lengths.push_back(3);

                // extract ngrams from text
                auto textDist = numeric::Distribution<std::string,int>::relative(util::NGrams::ngrams(text, lengths));

                // compare to each model
                std::map<std::string, double> retval;
                for(auto &pair : m_models)
                {
                    auto modelDist = numeric::Distribution<std::string,double>::relative(pair.second);
                    retval[pair.first] = compare(textDist, modelDist);
                }

                return retval;
            }

            /*!
             */
            virtual std::unique_ptr<XML::IElement> toXML() const
            {
                // root element
                auto root = std::unique_ptr<XML::IElement>(new XML::DefaultElementImpl("ngramlanguagemodel"));

                // add elements for each observed language
                for(auto &pair : m_models)
                {
                    auto languageNode = std::unique_ptr<XML::IElement>(new XML::DefaultElementImpl(pair.first));

                    // add elements for each observed ngram
                    for(auto &ngram : pair.second)
                    {
                        auto ngramNode = std::unique_ptr<XML::IElement>(new XML::DefaultElementImpl("ngram"));
                        ngramNode->setAttribute("count", std::to_string(ngram.second));
                        ngramNode->setAttribute("text", ngram.first);
                        languageNode->add(std::move(ngramNode));
                    }

                    root->add(std::move(languageNode));
                }

                // return
                return std::move(root);
            }

            /*!
             */
            virtual void fromXML(std::unique_ptr<XML::IElement> xml)
            {
                // root element

                // elements for each observed language
                for(int i=0; i<xml->countChildren(); i++)
                {
                    auto &languageNode = xml->getChild(i);
                    std::map<std::string, double> languageModel;
                    for(int j=0; j<languageNode.countChildren(); j++)
                    {
                        auto &ngramNode = languageNode.getChild(j);
                        languageModel[ngramNode.getAttribute("text")] = atof(ngramNode.getAttribute("count").c_str());
                    }
                    m_models[languageNode.getName()] = languageModel;
                }
                // elements for each observed ngram
            }

        private:
            // --- methods ---
            using LanguageModelType = std::map<std::string, double>;

            /*!
             */
            double compare(const LanguageModelType& lm0, const LanguageModelType& lm1) const
            {
                std::set<std::string> ngramsUnion;
                for(auto &pair : lm0)
                {
                    ngramsUnion.insert(pair.first);
                }
                for(auto &pair : lm1)
                {
                    ngramsUnion.insert(pair.first);
                }

                // cosine similarity metric with naive discounted smoothing
                auto alpha = 0.05;
                auto terms = 0.0;
                auto squaresA = 0.0;
                auto squaresB = 0.0;
                for(auto &ngram : ngramsUnion)
                {
                    auto f0 = lm0.find(ngram) != lm0.end() ? lm0.find(ngram)->second * (1.0 - alpha) : alpha;
                    auto f1 = lm1.find(ngram) != lm1.end() ? lm1.find(ngram)->second * (1.0 - alpha) : alpha;
                    terms += f0 * f1;
                    squaresA += (f0 * f0);
                    squaresB += (f1 * f1);
                }

                return terms / (sqrt(squaresA) * sqrt(squaresB));
            }

            // --- members ---
            std::map<std::string, LanguageModelType> m_models;
    };
}

#endif // NGRAMLANGUAGEDETECTOR_HPP