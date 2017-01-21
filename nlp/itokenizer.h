#ifndef ITOKENIZER_HPP
#define ITOKENIZER_HPP

#include "nlp/imodel.hpp"

#include <string>
#include <vector>

namespace nlp
{
    /*!
     */
    class ITokenizer : public IModel
    {
        public:
            explicit ITokenizer() = default;
            virtual ~ITokenizer() = default;

            /*! \return a tokenized std::string
             */
            virtual std::vector<int> tokenize(const std::string& s) const = 0;

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // ITOKENIZER_HPP
