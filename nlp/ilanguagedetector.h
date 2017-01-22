#ifndef ILANGUAGEDETECTOR_HPP
#define ILANGUAGEDETECTOR_HPP

#include "imodel.hpp"

#include <map>
#include <string>

namespace nlp
{
    /*! In natural language processing, language identification or language guessing is the problem of determining which natural language given content is in.
        Computational approaches to this problem view it as a special case of text categorization, solved with various statistical methods.
     */
    class ILanguageDetector : public IModel
    {
        public:
            /*! default constructor
             */
            explicit ILanguageDetector() = default;

            /*! default destructor
             */
            virtual ~ILanguageDetector() = default;

            /*! \return a ranking of all languages known to the model
             */
            virtual std::map<std::string, double> detect(const std::string& text) const = 0;

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // ILANGUAGEDETECTOR_HPP
