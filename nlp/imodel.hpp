#ifndef NLP_IMODEL_HPP
#define NLP_IMODEL_HPP

#include <set>

namespace nlp
{
    enum IModelOption
    {
        IGNORE_CASE,
        IGNORE_NON_ALPHANUMERIC,
        IGNORE_NON_DICTIONARY
    };

    /*! common abstract base class for NLP models
     */
    class IModel
    {
        public:

            /*! constructor
             */
            explicit IModel() = default;

            /*! destructor
             */
            virtual ~IModel() = default;

            /*! Set a flag on this IModel
             */
            void setOption(IModelOption opt)
            {
                opts.insert(opt);
            }

            /*! Unset a flag on this IModel
             */
            void unsetOption(IModelOption opt)
            {
                opts.erase(opt);
            }

            /*! \return true iff  the given IModelOption was set, false otherwise
             */
            bool isSet(IModelOption opt) const
            {
                return opts.find(opt) != opts.cend();
            }

        private:
            // --- methods ---
            // --- members ---
            std::set<IModelOption> opts;
    };
}

#endif // NLP_IMODEL_HPP