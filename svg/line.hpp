#pragma once
#ifndef LINE_HPP
#define LINE_HPP

#include "ielement.hpp"

namespace SVG
{

    /*! Defines a line
     */
    class Line final : public IElement
    {
        public:
            // --- Methods ---
            /*! Constructor
            	 \param[in] x1	start of the Line on the x-axis
            	 \param[in] y1	start of the Line on the y-axis
            	 \param[in] x2	end of the Line on the x-axis
            	 \param[in] y2	end of the Line on the y-axis
             */
            explicit Line(int x1, int y1, int x2, int y2)
            {
                m_x1 = x1;
                m_y1 = y1;
                m_x2 = x2;
                m_y2 = y2;
            }

            /*! Default destructor
             */
            virtual ~Line() = default;

            /*! Prohibit const copy constructor
             */
            Line(const Line&) = delete;

            /*! Prohibit copy constructor
             */
            Line(Line&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const Line&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(Line&) = delete;

        protected:

            // --- IElement ---
            virtual std::unique_ptr<XML::Element> toXML() const
            {
                std::unique_ptr<XML::Element> node = IElement::toXML();
                node.get()->setName("line");
                node.get()->setAttribute("x1", m_x1);
                node.get()->setAttribute("y1", m_y1);
                node.get()->setAttribute("x2", m_x2);
                node.get()->setAttribute("y2", m_y2);
                return node;
            }

        private:
            // --- Members ---
            int m_x1;       //!< The x-axis top-left corner of the line
            int m_y1;       //!< The y-axis top-left corner of the line
            int m_x2;       //!< The x-axis bottom-right corner of the line
            int m_y2;       //!< The y-axis bottom-right corner of the line
    };

}

#endif // LINE_HPP
