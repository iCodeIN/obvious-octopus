#ifndef DEFAULTELEMENTIMPL_HPP
#define DEFAULTELEMENTIMPL_HPP

#include "ielement.hpp"

#include <assert.h>
#include <istream>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace XML
{
    /*! default implementation of IElement
     */
    class DefaultElementImpl : public XML::IElement
    {
        public:
            /*! Constructor
            	 \param[in] name	the (local) name of the IElement (without any namespace prefix)
             */
            explicit DefaultElementImpl(const std::string &name)
                : m_name(name)
            {
            }

            /*! Default destructor
             */
            virtual ~DefaultElementImpl() = default;

            /*! Prohibit const copy constructor
             */
            DefaultElementImpl(const DefaultElementImpl&) = delete;

            /*! Prohibit copy constructor
             */
            DefaultElementImpl(DefaultElementImpl&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const DefaultElementImpl&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(DefaultElementImpl&) = delete;

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

            int countChildren() const override
            {
                return (int) m_children.size();
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

        private:
            // --- Methods ---
            // --- Members ---
            std::string 									m_name;			//!< name
            std::string 									m_text;			//!< text
            std::map<std::string, std::string> 		        m_attributes;	//!< attributes
            std::vector<std::unique_ptr<IElement>> 	        m_children;		//!< child-Elements of this Element
    };
}

#endif // DEFAULTELEMENTIMPL_HPP
