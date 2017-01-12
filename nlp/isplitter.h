#ifndef SPLITTER_HPP
#define SPLITTER_HPP

namespace nlp
{
    /*!
     */
    class ISplitter
    {
        public:
            explicit ISplitter() = default;
            virtual ~ISplitter() = default;

            /*! \return a tokenized std::string
             */
            virtual std::vector<std::string> split(std::string s) const = 0;

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // NAIVESPLITTER_HPP
