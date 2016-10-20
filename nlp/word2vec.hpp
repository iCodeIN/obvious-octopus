#pragma once
#ifndef WORD2VEC_H
#define WORD2VEC_H

#include "matrix/matrix.hpp"
#include "numeric/weightedsampling.hpp"

#include <functional>
#include <memory>

using namespace Matrix;
using namespace Numeric;

namespace NLP
{
    /*! Word2vec is a group of related models that are used to produce word embeddings.
        These models are shallow, two-layer neural networks that are trained to reconstruct linguistic contexts of words.
        Word2vec takes as its input a large corpus of text and produces a high-dimensional space (typically of several hundred dimensions), with each unique word in the corpus being assigned a corresponding vector in the space.
        Word vectors are positioned in the vector space such that words that share common contexts in the corpus are located in close proximity to one another in the space.
        Word2vec was created by a team of researchers led by Tomas Mikolov at Google. The algorithm has been subsequently analysed and explained by other researchers and a Bayesian version of the algorithm is proposed as well.
     */
    template <typename T>
    class Word2Vec final
    {
        public:
            using UnigramGetterType       = std::function<const T&(int sample, int index)>;
            using SampleSizeGetterType    = std::function<int(int sampleIndex)>;

            explicit Word2Vec(const UnigramGetterType &unigramGetter, const SampleSizeGetterType &sampleSizeGetter, int nofSamples)
                : m_unigramGetter(unigramGetter)
                , m_sampleSizeGetter(sampleSizeGetter)
                , m_nofSamples(nofSamples)
            {

                // build dictionary and distribution
                std::map<long, double> distribution;
                using relationType = typename boost::bimap<T,long>::value_type;
                for(int i = 0 ; i < nofSamples ; i++)
                {
                    for(int j = 0 ; j < m_sampleSizeGetter(i) ; j++)
                    {
                        auto key = m_unigramGetter(i, j);
                        auto ind = ( m_dictionary.left.find(key) == m_dictionary.left.end() ) ? m_dictionary.left.size() : m_dictionary.left.find(key)->second;
                        if(distribution[ind] == 0)
                        {
                            m_dictionary.insert(relationType(key, ind));
                        }
                        distribution[ind]++;
                    }
                }

                // build unigram distribution
                m_unigramLookupTable = std::unique_ptr<WeightedSampling<T>>(new WeightedSampling<T>(m_dictionary, distribution));

                // set up matrix of weights
                auto r = distribution.size();
                auto c = m_nofDimensions;
                m_syn0 = Matrix::randMatrix(r, c);
                m_syn1 = Matrix::randMatrix(r, c);
            }

            using LearningRateFunctionType = std::function<double(int epoch, double prevLearningRate)>;
            void train(int nofIterations, const LearningRateFunctionType &learningRateFunction)
            {
                // initialize learning rate
                m_learningRate = 1.0;

                // iterations
                for(int i=0; i<nofIterations; i++)
                {
                    // set learning rate
                    m_learningRate = learningRateFunction(i, m_learningRate);

                    // samples
                    for(int j=0; j<m_nofSamples; j++)
                    {
                        // all items in sample
                        for(int k = 0 ; k < m_sampleSizeGetter(j) ; k++)
                        {
                            auto &sObj = m_unigramGetter(j, k);
                            auto  sInd = m_dictionary[sObj];

                            // positive samples
                            for(int l = 0 ; l < m_sampleSizeGetter(j) ; l++)
                            {
                                auto &tObj = m_unigramGetter(j, l);
                                trainPair(sInd, m_dictionary[tObj], 1);
                            }
                            // negative samples
                            for(int l = 0 ; l < m_nofNegativeSamples ; l++)
                            {
                                auto &tObj = m_unigramLookupTable->pick();
                                trainPair(sInd, m_dictionary[tObj], 0);
                            }
                        }
                    }
                }
            }

        private:
            // --- methods ---
            void trainPair(long source, long target, long label)
            {
                // define sigmoid
                auto sigmoid = [](double x)
                {
                    return 1.0 / (1.0 + exp(-x));
                };

                // reference to matrices
                auto &syn0 = *(m_syn0.get());
                auto &syn1 = *(m_syn1.get());

                // calculate error
                auto f = 0.0;
                for(int i=0; i<m_nofDimensions; i++)
                {
                    f += Matrix::get(syn0, source, i) * Matrix::get(syn1, target, i);
                }

                // calculate gradient
                auto g = sigmoid(f);

                // adjust weight matrix based on gradient
            }

            // --- members ---
            boost::bimap<T, long>                                    m_dictionary;
            std::unique_ptr<Numeric::WeightedSampling<T>>            m_unigramLookupTable;
            std::unique_ptr<MatrixType>                              m_syn0;
            std::unique_ptr<MatrixType>                              m_syn1;

            const UnigramGetterType                                 &m_unigramGetter;              //!< access to unigrams
            const SampleSizeGetterType                              &m_sampleSizeGetter;           //!< access to size of samples
            const int                                                m_nofSamples;                 //!< number of samples

            const int                                                m_nofNegativeSamples = 5;     //!< number of negative samples
            const int                                                m_nofDimensions = 100;        //!< number of dimensions of produced vectors
            const double                                             m_learningRate = 0.01;        //!< learning rate
    };
}

#endif // WORD2VEC_H
