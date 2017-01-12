#ifndef NAIVESPLITTER_HPP
#define NAIVESPLITTER_HPP

#include "nlp/isplitter.h"

namespace nlp
{
    /*! ISplitter implementation using a dictionary-based algorithm
     */
    class NaiveSplitter : public ISplitter
    {
        public:

            /*! constructor
             */
            explicit NaiveSplitter(std::set<std::string> dictionary)
            {
            }

            /*! destructor
             */
            virtual ~NaiveSplitter() = default;

            // --- ISplitter ---
            std::vector<std::string> split(std::string s) const override
            {

            }

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // NAIVESPLITTER_HPP