#ifndef ISTEMMER_HPP
#define ISTEMMER_HPP

#include "nlp/imodel.hpp"

#include <string>

namespace nlp
{
    /*! In linguistic morphology and information retrieval, stemming is the process of reducing inflected (or sometimes derived) words to their word stem, base or root form—generally a written word form.
        The stem need not be identical to the morphological root of the word;
        it is usually sufficient that related words map to the same stem, even if this stem is not in itself a valid root.
        Algorithms for stemming have been studied in computer science since the 1960s.
        Many search engines treat words with the same stem as synonyms as a kind of query expansion, a process called conflation.
     */
    class IStemmer : public IModel
    {
        public:
            /*! default constructor
             */
            explicit IStemmer() = default;

            /*! default destructor
             */
            virtual ~IStemmer() = default;

            //! \return the stem of the given std::string
            virtual std::string stem(const std::string& s) const = 0;

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // ISTEMMER_HPP