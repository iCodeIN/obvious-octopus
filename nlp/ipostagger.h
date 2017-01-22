#ifndef IPOSTAGGER_HPP
#define IPOSTAGGER_HPP

#include "nlp/imodel.hpp"

namespace nlp
{
    /*! In corpus linguistics, part-of-speech tagging (POS tagging or POST), also called grammatical tagging or word-category disambiguation,
        is the process of marking up a word in a text (corpus) as corresponding to a particular part of speech,
        based on both its definition and its context—i.e., its relationship with adjacent and related words in a phrase, sentence, or paragraph.
        A simplified form of this is commonly taught to school-age children, in the identification of words as nouns, verbs, adjectives, adverbs, etc.
     */
    class IPOSTagger : public IModel
    {
        public:
            /*! default constructor
             */
            explicit IPOSTagger() = default;

            /*! default destructor
             */
            virtual ~IPOSTagger() = default;
        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // IPOSTAGGER_HPP
