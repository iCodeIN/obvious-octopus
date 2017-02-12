#pragma once
#ifndef SVG_LINE_HPP
#define SVG_LINE_HPP

#include "svg/ielement.hpp"

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

            /*! Sets the stroke-dasharray attribute
                This attribute controls the pattern of dashes and gaps used to stroke this Line
             */
            void setDashArray(const std::vector<int>& pattern)
            {
                m_dashArray = pattern;
            }

            /*! Reset the stroke-dasharray attribute to solid line
             */
            void clearDashArray()
            {
                m_dashArray.clear();
            }

        protected:

            // --- IElement ---
            virtual std::unique_ptr<XML::IElement> toXML() const
            {
                auto node = IElement::toXML();
                node->setName("line");
                node->setAttribute("x1", m_x1);
                node->setAttribute("y1", m_y1);
                node->setAttribute("x2", m_x2);
                node->setAttribute("y2", m_y2);
                if(!m_dashArray.empty())
                {
                    std::string strokeArrayString = "";
                    for(int i=0; i<m_dashArray.size(); i++)
                    {
                        strokeArrayString += (i == 0 ? "" : ", ") + std::to_string(m_dashArray[i]);
                    }
                    node->setAttribute("stroke-dasharray", strokeArrayString);
                }
                return node;
            }

        private:
            // --- Members ---
            std::vector<int>    m_dashArray;  //!< The stroke array attribute of the line
            int                 m_x1;           //!< The x-axis top-left corner of the line
            int                 m_y1;           //!< The y-axis top-left corner of the line
            int                 m_x2;           //!< The x-axis bottom-right corner of the line
            int                 m_y2;           //!< The y-axis bottom-right corner of the line
    };

}

#endif // SVG_LINE_HPP
