#ifndef NLP_ITOKENIZER_HPP
#define NLP_ITOKENIZER_HPP

#include "nlp/imodel.hpp"

#include <string>
#include <vector>

namespace nlp
{
    /*! In lexical analysis, tokenization is the process of breaking a stream of text up into words, phrases, symbols, or other meaningful elements called tokens.
        The list of tokens becomes input for further processing such as parsing or text mining.
        Tokenization is useful both in linguistics (where it is a form of text segmentation), and in computer science, where it forms part of lexical analysis.
     */
    class ITokenizer : public IModel
    {
        public:
            /*! default constructor
             */
            explicit ITokenizer() = default;

            /*! default destructor
             */
            virtual ~ITokenizer() = default;

            /*! \return a tokenized std::string
             */
            virtual std::vector<int> tokenize(const std::string& s) const = 0;

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // NLP_ITOKENIZER_HPP
