#ifndef XML_IELEMENT_HPP
#define XML_IELEMENT_HPP

#include <istream>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

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
    class IElement
    {
        public:
            /*! Constructor
            	 \param[in] name	the (local) name of the Element (without any namespace prefix)
             */
            explicit IElement() = default;

            /*! Default destructor
             */
            virtual ~IElement() = default;

            /*! Prohibit const copy constructor
             */
            IElement(const IElement&) = delete;

            /*! Prohibit copy constructor
             */
            IElement(IElement&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const IElement&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(IElement&) = delete;

            /*! Set the (local) name of this Element (without any namespace prefix).
             */
            virtual void setName(const std::string &name) = 0;

            /*! \return the (local) name of this Element (without any namespace prefix).
             */
            virtual const std::string getName() const = 0;

            /*! Sets the content of this Element to the given text.
            	 \param[in] text	the new content
             */
            virtual void setText(const std::string &text) = 0;

            /*! \return the content of this Element
             */
            virtual  const std::string getText() const = 0;

            /*! Appends the given Element to the end of this Element's content list.
            	 \param[in] n	the (child) Element to be added
             */
            virtual void add(std::unique_ptr<IElement> n) = 0;

            /*! Removes the Element at given index from this Element's content list.
            	 \param[in] i	the index of the (child) Element to be removed
             */
            virtual void remove(int i) = 0;

            /*! \returns the Element at given index in this Element's content list.
            	 \param[in] i	the index of the (child) Element
             */
            virtual const IElement& getChild(int i) const = 0;

            /*! \returns true if there is an (child) Element at given index in this Element's content list.
            	 \param[in] i	the index of the (child) Element
             */
            virtual bool hasChild(int i) const = 0;

            /*! Sets the attribute/value pair for this Element
            	 \param[in] attribute 	the name of the attribute being set
            	 \param[in] value			the value of the attribute being set
             */
            virtual void setAttribute(const std::string &attribute, const std::string &value) = 0;

            /*! Sets the attribute/value pair for this Element
            	 \param[in] attribute 	the name of the attribute being set
            	 \param[in] value			the value of the attribute being set
             */
            virtual void setAttribute(const std::string &attribute, int value) = 0;

            /*! Sets the attribute/value pair for this Element
            	 \param[in] attribute 	the name of the attribute being set
            	 \param[in] value			the value of the attribute being set
             */
            virtual void setAttribute(const std::string &attribute, double value) = 0;

            /*! Sets the attribute/value pair for this Element
            	 \param[in] attribute 	the name of the attribute being set
            	 \param[in] value			the value of the attribute being set
             */
            virtual void setAttribute(const std::string &attribute, long value) = 0;

            /*! Sets the attribute/value pair for this Element
            	 \param[in] attribute 	the name of the attribute being set
            	 \param[in] value			the value of the attribute being set
             */
            virtual void setAttribute(const std::string &attribute, float value) = 0;

            /*! \return true if this Element has an attribute with given name
            	 \param[in] attribute 	the name of the attribute to check for
             */
            virtual bool hasAttribute(const std::string &attribute) const = 0;

            /*! \return the value of the attribute with given name
            	 \param[in] attribute	the name of the attribute to check for
             */
            virtual const std::string getAttribute(const std::string &attribute) const = 0;

            /*! \return all attribute/value pairs
             */
            virtual const std::map<std::string, std::string> getAttributes() const = 0;

    };
}

#endif // XML_IELEMENT_HPP
