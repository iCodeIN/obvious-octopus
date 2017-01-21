#ifndef IPOSTAGGER_HPP
#define IPOSTAGGER_HPP

#include "nlp/imodel.hpp"

namespace nlp
{
    class IPOSTagger : public IModel
    {
        public:
            explicit IPOSTagger() = default;
            virtual ~IPOSTagger() = default;
        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // IPOSTAGGER_HPP
