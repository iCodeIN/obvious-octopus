#ifndef NLP_DICTIONARY_HPP
#define NLP_DICTIONARY_HPP

#include "graph/bktree.hpp"
#include "util/levenshtein.hpp"
#include "util/stringutils.hpp"
#include "nlp/imodel.hpp"
#include "xml/xml.hpp"

#include <functional>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <iostream> // debug

namespace nlp
{
    /*! Utility class to handle word (including prefix, suffix, and quasi-match) lookup
     */
    class Dictionary : public IModel
    {
        public:

            /*! default constructor
             */
            explicit Dictionary()
            {
                // init BKTree
                auto m = [](const std::string& s0, const std::string& s1)
                {
                    return util::string::levenshtein(s0, s1);
                };
                m_bktree = std::unique_ptr<graph::BKTree<std::string>>(new graph::BKTree<std::string>(m));
            }

            /*! default constructor
             */
            virtual ~Dictionary() = default;

            /*! \return true iff the given std::string was added successfully to the dictionary
             */
            bool insert(const std::string& s)
            {
                // insert word
                m_words.insert(s);
                m_bktree->insert(s);

                for(int i=0; i<s.size(); i++)
                {
                    // insert prefix
                    auto prefix = s.substr(0, i);

                    // insert suffix
                    auto suffix = s.substr(i+1);

                    m_prefixes.insert(prefix);
                    m_suffixes.insert(suffix);
                }

                // return
                return true;
            }

            /*! \return true iff the given std::string is a valid dictionary word
             */
            bool isWord(const std::string& s) const
            {

                return this->isSet(IGNORE_CASE) ? (m_words.find(util::string::toUpper(s)) != m_words.end()) : (m_words.find(s) != m_words.end());
            }

            /*! \return true iff the given std::string is a prefix of a valid dictionary word
             */
            bool isPrefix(const std::string& s) const
            {
                return this->isSet(IGNORE_CASE) ? (m_prefixes.find(util::string::toUpper(s)) != m_prefixes.end()) : (m_prefixes.find(s) != m_prefixes.end());
            }

            /*! \return true iff the given std::string is a suffix of a valid dictionary word
             */
            bool isSuffix(const std::string& s) const
            {
                return this->isSet(IGNORE_CASE) ? (m_suffixes.find(util::string::toUpper(s)) != m_suffixes.end()) : (m_suffixes.find(s) != m_suffixes.end());
            }

            /*! \return true iff the given std::string is almost a valid dictionary word
                i.e. if the given std::string is at a given distance from any dictionary word
             */
            bool isPseudoWord(const std::string& s, int dist) const
            {
                return this->isSet(IGNORE_CASE) ? !m_bktree->find(util::string::toUpper(s), dist).empty() : !m_bktree->find(s, dist).empty();
            }

            /*! load a Dictionary object from an XML file
             */
            void fromXML(std::unique_ptr<XML::IElement> xmlRoot)
            {
                // elements for each observed language
                for(int i=0; i<xmlRoot->countChildren(); i++)
                {
                    auto word = xmlRoot->getChild(i).getText();
                    if(this->isSet(IGNORE_CASE))
                    {
                        word = util::string::toUpper(word);
                    }
                    insert(word);
                    if(i%1000==0)
                    {
                        std::cout << i << " / " << xmlRoot->countChildren() << std::endl;
                    }
                }
            }

        private:
            // --- methods ---
            // --- members ---
            std::unique_ptr<graph::BKTree<std::string>>     m_bktree;
            std::set<std::string>                           m_words;
            std::set<std::string>                           m_prefixes;
            std::set<std::string>                           m_suffixes;
    };
}

#endif // NLP_DICTIONARY_HPP