#pragma once
#ifndef SVG_RECTANGLE_HPP
#define SVG_RECTANGLE_HPP

#include "svg/ielement.hpp"

#include <assert.h>

namespace SVG
{

    /*! Defines a rectangle
     */
    class Rectangle final : public IElement
    {
        public:
            // --- Methods ---
            /*! Constructor
            	 \param[in] x			x-coordinate of the upper-left corner of the Rectangle
            	 \param[in] y			y-coordinate of the upper-left corner of the Rectangle
            	 \param[in] width		width of the Rectangle
            	 \param[in] height	height of the Rectangle
             */
            explicit Rectangle(int x, int y, int width, int height)
            {
                assert(width >= 0 && height >= 0);
                m_x = x;
                m_y = y;
                m_rx = 0;
                m_ry = 0;
                m_width = width;
                m_height = height;
            }
            /*! Constructor
            	 \param[in] x			x-coordinate of the upper-left corner of the Rectangle
            	 \param[in] y			y-coordinate of the upper-left corner of the Rectangle
            	 \param[in] width		width of the Rectangle
            	 \param[in] height	height of the Rectangle
            	 \param[in] rx			x-axis rounding of the corners of the Rectangle
            	 \param[in] ry			y-axis rounding of the corners of the Rectangle
             */
            explicit Rectangle(int x, int y, int width, int height, int rx, int ry)
            {
                assert(width >= 0 && height >= 0 && rx >= 0 && ry >= 0);
                m_x = x;
                m_y = y;
                m_rx = rx;
                m_ry = ry;
                m_width = width;
                m_height = height;
            }

            /*! Default destructor
             */
            virtual ~Rectangle() = default;

            /*! Prohibit const copy constructor
             */
            Rectangle(const Rectangle&) = delete;

            /*! Prohibit copy constructor
             */
            Rectangle(Rectangle&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const Rectangle&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(Rectangle&) = delete;

        protected:
            // --- IElement ---
            virtual std::unique_ptr<XML::IElement> toXML() const
            {
                auto node = IElement::toXML();
                node.get()->setName("rect");
                node.get()->setAttribute("x", m_x);
                node.get()->setAttribute("y", m_y);
                node.get()->setAttribute("rx", m_rx);
                node.get()->setAttribute("ry", m_ry);
                node.get()->setAttribute("width", m_width);
                node.get()->setAttribute("height", m_height);
                return node;
            }

        private:
            // --- Members ---
            int m_x;       //!< The x-axis top-left corner of the rectangle
            int m_y;       //!< The y-axis top-left corner of the rectangle
            int m_rx;      //!< The x-axis radius (to round the element)
            int m_ry;      //!< The y-axis radius (to round the element)
            int m_width;   //!< The width of the rectangle
            int m_height;  //!< The height of the rectangle
    };

}

#endif // SVG_RECTANGLE_HPP
