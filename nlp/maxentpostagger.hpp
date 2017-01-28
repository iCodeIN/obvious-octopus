#ifndef MAXENTPOSTAGGER_HPP
#define MAXENTPOSTAGGER_HPP

#include "nlp/ipostagger.h"

namespace nlp
{
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

#endif // MAXENTPOSTAGGER_HPP