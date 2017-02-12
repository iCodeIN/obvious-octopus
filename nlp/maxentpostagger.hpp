#ifndef NLP_MAXENTPOSTAGGER_HPP
#define NLP_MAXENTPOSTAGGER_HPP

#include "nlp/ipostagger.h"

namespace nlp
{
    /*! In machine learning, a maximum-entropy Markov model (MEMM), or conditional Markov model (CMM),
        is a graphical model for sequence labeling that combines features of hidden Markov models (HMMs) and maximum entropy (MaxEnt) models.
        An MEMM is a discriminative model that extends a standard maximum entropy classifier by assuming that the unknown values to be learnt
        are connected in a Markov chain rather than being conditionally independent of each other.
        MEMMs find applications in natural language processing, specifically in part-of-speech tagging.
     */
    class MaxEntPOSTagger : public IPOSTagger
    {
        public:
            /*! default constructor
             */
            explicit MaxEntPOSTagger() = default;

            /*! default destructor
             */
            virtual ~MaxEntPOSTagger() = default;

            //! --- IPOSTagger ---
            virtual std::vector<std::string> tag(const std::vector<std::string>& tokens) const
            {

            }

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // NLP_MAXENTPOSTAGGER_HPP