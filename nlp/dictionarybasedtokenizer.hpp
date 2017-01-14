#ifndef DICTIONARYBASEDTOKENIZER_HPP
#define DICTIONARYBASEDTOKENIZER_HPP

#include "nlp/itokenizer.h"

namespace nlp
{
    /*! ITokenizer implementation using a dictionary-based algorithm
     */
    class DictionaryBasedTokenizer : public ITokenizer
    {
        public:

            /*! constructor
             */
            explicit DictionaryBasedTokenizer()
            {
            }

            /*! destructor
             */
            virtual ~DictionaryBasedTokenizer() = default;

            // --- ITokenizer ---
            std::vector<std::string> tokenize(std::string s) const override
            {

            }

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // DICTIONARYBASEDTOKENIZER_HPP