#ifndef ELEMENT_HPP
#define ELEMENT_HPP

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
    /*! Extensible Markup Language (XML) is a markup language that defines a set of rules for encoding documents in a format that is both human-readable and machine-readable. It is defined by
    	 the W3C's XML 1.0 Specification and by several other related specifications, all of which are free open standards.
    	 The design goals of XML emphasize simplicity, generality and usability across the Internet. It is a textual data format with strong support via Unicode for different human languages.
    	 Although the design of XML focuses on documents, it is widely used for the representation of arbitrary data structures such as those used in web services.
    	 Several schema systems exist to aid in the definition of XML-based languages, while many application programming interfaces (APIs) have been developed to aid the processing of XML data.

    	 A logical document component that either begins with a start-tag and ends with a matching end-tag or consists only of an empty-element tag. The characters between the start- and
    	 end-tags, if any, are the element's content, and may contain markup, including other elements, which are called child elements. An example of an element is
    	 '&lt;Greeting&gt;Hello, world.&lt;/Greeting&gt;'. Another is '&lt;line-break /&gt;'.
     */
    class Element final
    {
        public:
            /*! Constructor
            	 \param[in] name	the (local) name of the Element (without any namespace prefix)
             */
            explicit Element(const std::string &name)
                : m_name(name)
            {
            }

            /*! Default destructor
             */
            virtual ~Element() = default;

            /*! Prohibit const copy constructor
             */
            Element(const Element&) = delete;

            /*! Prohibit copy constructor
             */
            Element(Element&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const Element&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(Element&) = delete;

            /*! Set the (local) name of this Element (without any namespace prefix).
             */
            void setName(const std::string &name)
            {
                m_name = name;
            }

            /*! \return the (local) name of this Element (without any namespace prefix).
             */
            std::string getName() const
            {
                return m_name;
            }

            /*! Sets the content of this Element to the given text.
            	 \param[in] text	the new content
             */
            void setText(const std::string &text)
            {
                m_text = text;
            }

            /*! \return the content of this Element
             */
            std::string getText() const
            {
                return m_text;
            }

            /*! Appends the given Element to the end of this Element's content list.
            	 \param[in] n	the (child) Element to be added
             */
            void add(std::unique_ptr<Element> n)
            {
                auto rawPtr = n.get();
                assert(rawPtr != this);
                m_children.push_back(std::move(n));
            }

            /*! Removes the Element at given index from this Element's content list.
            	 \param[in] i	the index of the (child) Element to be removed
             */
            void remove(int i)
            {
                assert(hasChild(i));
                auto it = m_children.begin() + i;
                m_children.erase(it);
            }

            /*! \returns the Element at given index in this Element's content list.
            	 \param[in] i	the index of the (child) Element
             */
            const Element& getChild(int i) const
            {
                assert(hasChild(i));
                return *m_children[i].get();
            }

            /*! \returns true if there is an (child) Element at given index in this Element's content list.
            	 \param[in] i	the index of the (child) Element
             */
            bool hasChild(int i) const
            {
                return m_children.size() > i;
            }

            /*! Sets the attribute/value pair for this Element
            	 \param[in] attribute 	the name of the attribute being set
            	 \param[in] value			the value of the attribute being set
             */
            void setAttribute(const std::string &attribute, const std::string &value)
            {
                m_attributes[attribute] = value;
            }

            /*! Sets the attribute/value pair for this Element
            	 \param[in] attribute 	the name of the attribute being set
            	 \param[in] value			the value of the attribute being set
             */
            void setAttribute(const std::string &attribute, int value)
            {
                std::ostringstream stream;
                stream << value;
                setAttribute(attribute, stream.str());
            }

            /*! Sets the attribute/value pair for this Element
            	 \param[in] attribute 	the name of the attribute being set
            	 \param[in] value			the value of the attribute being set
             */
            void setAttribute(const std::string &attribute, double value)
            {
                std::ostringstream stream;
                stream << value;
                setAttribute(attribute, stream.str());
            }

            /*! Sets the attribute/value pair for this Element
            	 \param[in] attribute 	the name of the attribute being set
            	 \param[in] value			the value of the attribute being set
             */
            void setAttribute(const std::string &attribute, long value)
            {
                std::ostringstream stream;
                stream << value;
                setAttribute(attribute, stream.str());
            }

            /*! Sets the attribute/value pair for this Element
            	 \param[in] attribute 	the name of the attribute being set
            	 \param[in] value			the value of the attribute being set
             */
            void setAttribute(const std::string &attribute, float value)
            {
                std::ostringstream stream;
                stream << value;
                setAttribute(attribute, stream.str());
            }

            /*! \return true if this Element has an attribute with given name
            	 \param[in] attribute 	the name of the attribute to check for
             */
            bool hasAttribute(const std::string &attribute) const
            {
                return m_attributes.find(attribute) != m_attributes.end();
            }

            /*! \return the value of the attribute with given name
            	 \param[in] attribute	the name of the attribute to check for
             */
            std::string getAttribute(const std::string &attribute) const
            {
                assert(hasAttribute(attribute));
                auto it = m_attributes.find(attribute);
                return it->second;
            }

            /*! \return all attribute/value pairs
             */
            const std::map<std::string, std::string> getAttributes() const
            {
                return m_attributes;
            }

            /*! Writes an Element to an std::ostream
             	 \param[in] os	std::ostream to write the XML::Element to
            	 \param[in] e	XML::Element to stream
             */
            friend std::ostream& operator<<(std::ostream& os, const XML::Element& e)
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
            friend std::istream& operator>>(std::istream& is, XML::Element& e)
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
            static void toTree(const XML::Element &Element, const std::string &path, boost::property_tree::ptree &tree)
            {
                std::string newPath = path.empty() ? Element.getName() : path + "." + Element.getName();

                // put text
                auto &it = tree.add(newPath, Element.getText());

                // process attributes
                for(auto pair : Element.getAttributes())
                {
                    it.add("<xmlattr>." + pair.first, pair.second);
                }

                // process children
                int i = 0;
                while(Element.hasChild(i))
                {
                    toTree(Element.getChild(i), newPath, tree);
                    i++;
                }
            }

            /*!
             */
            static void fromTree(XML::Element &element, const boost::property_tree::ptree &tree)
            {
                /* The first level of a boost::property_tree::ptree is always empty.
                	When the recursion 'fromTree' start, the element does not have a name.
                	This condition enables the algorithm to skip the empty root and move on to the 'real' root of the XML tree.
                 */
                if(element.getName().empty())
                {
                    auto realRoot = *(tree.begin());
                    element.setName(realRoot.first);
                    fromTree(element, realRoot.second);
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
                            element.setAttribute(attrChild.first, attrChild.second.get_value<std::string>());
                        }
                    }
                    // else we are dealing with regular XML child-nodes
                    else
                    {
                        std::unique_ptr<XML::Element> childPtr = std::unique_ptr<XML::Element>(new XML::Element(name));
                        fromTree(*childPtr.get(), child.second);
                        element.add(std::move(childPtr));
                    }
                }

            }
            // --- Members ---
            std::string 									m_name;			//!< name
            std::string 									m_text;			//!< text
            std::map<std::string, std::string> 		        m_attributes;	//!< attributes
            std::vector<std::unique_ptr<Element>> 	        m_children;		//!< child-Elements of this Element
    };
}

#endif // Element_HPP
