#ifndef BOOSTELEMENTIMPL_HPP
#define BOOSTELEMENTIMPL_HPP

#include "xml/ielement.hpp"

#include <assert.h>
#include <istream>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace XML
{
    /*! Implementation of IElement using boost for serialization
     */
    class BoostElementImpl : public XML::IElement
    {
        public:
            /*! Constructor
            	 \param[in] name	the (local) name of the IElement (without any namespace prefix)
             */
            explicit BoostElementImpl(const std::string &name)
                : m_name(name)
            {
            }

            /*! Default destructor
             */
            virtual ~BoostElementImpl() = default;

            /*! Prohibit const copy constructor
             */
            BoostElementImpl(const BoostElementImpl&) = delete;

            /*! Prohibit copy constructor
             */
            BoostElementImpl(BoostElementImpl&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const BoostElementImpl&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(BoostElementImpl&) = delete;

            // --- IElement ---
            void setName(const std::string &name) override
            {
                m_name = name;
            }

            // --- IElement ---
            const std::string getName() const override
            {
                return m_name;
            }

            // --- IElement ---
            void setText(const std::string &text) override
            {
                m_text = text;
            }

            // --- IElement ---
            const std::string getText() const override
            {
                return m_text;
            }

            // --- IElement ---
            void add(std::unique_ptr<XML::IElement> n) override
            {
                auto rawPtr = n.get();
                assert(rawPtr != this);
                m_children.push_back(std::move(n));
            }

            // --- IElement ---
            void remove(int i) override
            {
                assert(hasChild(i));
                auto it = m_children.begin() + i;
                m_children.erase(it);
            }

            // --- IElement ---
            const XML::IElement& getChild(int i) const override
            {
                assert(hasChild(i));
                return *m_children[i].get();
            }

            // --- IElement ---
            bool hasChild(int i) const override
            {
                return m_children.size() > i;
            }

            // --- IElement ---
            void setAttribute(const std::string &attribute, const std::string &value) override
            {
                m_attributes[attribute] = value;
            }

            // --- IElement ---
            void setAttribute(const std::string &attribute, int value) override
            {
                std::ostringstream stream;
                stream << value;
                setAttribute(attribute, stream.str());
            }

            // --- IElement ---
            void setAttribute(const std::string &attribute, double value) override
            {
                std::ostringstream stream;
                stream << value;
                setAttribute(attribute, stream.str());
            }

            // --- IElement ---
            void setAttribute(const std::string &attribute, long value) override
            {
                std::ostringstream stream;
                stream << value;
                setAttribute(attribute, stream.str());
            }

            // --- IElement ---
            void setAttribute(const std::string &attribute, float value) override
            {
                std::ostringstream stream;
                stream << value;
                setAttribute(attribute, stream.str());
            }

            // --- IElement ---
            bool hasAttribute(const std::string &attribute) const override
            {
                return m_attributes.find(attribute) != m_attributes.end();
            }

            // --- IElement ---
            const std::string getAttribute(const std::string &attribute) const override
            {
                assert(hasAttribute(attribute));
                auto it = m_attributes.find(attribute);
                return it->second;
            }

            // --- IElement ---
            const std::map<std::string, std::string> getAttributes() const override
            {
                return m_attributes;
            }

            /*! Writes an Element to an std::ostream
             	 \param[in] os	std::ostream to write the XML::Element to
            	 \param[in] e	XML::Element to stream
             */
            friend std::ostream& operator<<(std::ostream& os, const XML::IElement* e)
            {
                boost::property_tree::ptree tree;
                toTree(e, "", tree);
                boost::property_tree::write_xml(os, tree, boost::property_tree::xml_writer_make_settings<std::string>('	', 1));
                return os;
            }

            /*! Reads an Element from an std::istream
             	 \param[in] is	std::istream to read the XML::Element from
            	 \param[in] e	XML::Element to stream
             */
            friend std::istream& operator>>(std::istream& is, XML::IElement* e)
            {
                boost::property_tree::ptree tree;
                boost::property_tree::read_xml(is, tree);
                fromTree(e, tree);
                return is;
            }

        private:
            // --- Methods ---

            /*!
             */
            static void toTree(const XML::IElement* e, const std::string &path, boost::property_tree::ptree &tree)
            {
                std::string newPath = path.empty() ? e->getName() : path + "." + e->getName();

                // put text
                auto &it = tree.add(newPath, e->getText());

                // process attributes
                for(auto pair : e->getAttributes())
                {
                    it.add("<xmlattr>." + pair.first, pair.second);
                }

                // process children
                int i = 0;
                while(e->hasChild(i))
                {
                    toTree(&(e->getChild(i)), newPath, tree);
                    i++;
                }
            }

            /*!
             */
            static void fromTree(XML::IElement* e, const boost::property_tree::ptree &tree)
            {
                /* The first level of a boost::property_tree::ptree is always empty.
                	When the recursion 'fromTree' start, the element does not have a name.
                	This condition enables the algorithm to skip the empty root and move on to the 'real' root of the XML tree.
                 */
                if(e->getName().empty())
                {
                    auto realRoot = *(tree.begin());
                    e->setName(realRoot.first);
                    fromTree(e, realRoot.second);
                    return;
                }

                // process children
                for(auto child : tree)
                {
                    auto name = child.first;

                    // if the name is '<xmlattr>' then we are dealing with XML attributes
                    if(name == "<xmlattr>")
                    {
                        // process attributes
                        for(auto attrChild : child.second)
                        {
                            e->setAttribute(attrChild.first, attrChild.second.get_value<std::string>());
                        }
                    }
                    // else we are dealing with regular XML child-nodes
                    else
                    {
                        auto childPtr = std::unique_ptr<XML::IElement>(new XML::BoostElementImpl(name));
                        fromTree(childPtr.get(), child.second);
                        e->add(std::move(childPtr));
                    }
                }

            }
            // --- Members ---
            std::string 									m_name;			//!< name
            std::string 									m_text;			//!< text
            std::map<std::string, std::string> 		        m_attributes;	//!< attributes
            std::vector<std::unique_ptr<IElement>> 	        m_children;		//!< child-Elements of this Element
    };
}

#endif // Element_HPP
