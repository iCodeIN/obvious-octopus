#pragma once
#ifndef SVG_TEXT_HPP
#define SVG_TEXT_HPP

#include "svg/ielement.hpp"

namespace SVG
{
    /*! Defines a piece of text being drawn on the canvas
     */
    class Text : public IElement
    {
        public:
            /*! constructor
             */
            explicit Text(int x, int y, const std::string& text)
            {
                m_x = x;
                m_y = y;
                m_text = text;
            }

            /*! Default destructor
             */
            virtual ~Text() = default;

            /*! Prohibit const copy constructor
             */
            Text(const Text&) = delete;

            /*! Prohibit copy constructor
             */
            Text(Text&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const Text&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(Text&) = delete;

        protected:
            // --- IElement ---
            virtual std::unique_ptr<XML::IElement> toXML() const
            {
                auto node = IElement::toXML();
                node->setName("text");
                node->setAttribute("x", m_x);
                node->setAttribute("y", m_y);
                node->setText("text", m_text);
                return node;
            }

        private:
            // --- members ---
            std::string m_text;
            int         m_x;       //!< The x-axis top-left corner of the rectangle
            int         m_y;       //!< The y-axis top-left corner of the rectangle
    };
}

#endif SVG_TEXT_HPP