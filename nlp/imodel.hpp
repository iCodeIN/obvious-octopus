#ifndef IMODEL_HPP
#define IMODEL_HPP

#include <set>

namespace nlp
{
    enum IModelOption
    {
        IGNORE_CASE,
        IGNORE_NON_ALPHANUMERIC,
        IGNORE_NON_DICTIONARY
    };

    class IModel
    {
        public:

            /*! constructor
             */
            explicit IModel() = default;

            /*! destructor
             */
            virtual ~IModel() = default;

            void setOption(IModelOption opt)
            {
                opts.insert(opt);
            }

            void unsetOption(IModelOption opt)
            {
                opts.erase(opt);
            }

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

#endif // IMODEL_HPP