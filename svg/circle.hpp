#pragma once
#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "ielement.hpp"

#include <assert.h>

namespace SVG
{
    /*! Defines a circle
     */
    class Circle final : public IElement
    {
        public:
            // --- Methods ---
            /*! Constructor
            	 \param[in] cx		x-coordinate of the center of the Circle
            	 \param[in] cy		y-coordinate of the center of the Circle
            	 \param[in] r		radius of the Circle
             */
            explicit Circle(int cx, int cy, int r)
            {
                assert(r >= 0);
                m_cx = cx;
                m_cy = cy;
                m_r = r;
            }

            /*! Default destructor
             */
            virtual ~Circle() = default;

            /*! Prohibit const copy constructor
             */
            Circle(const Circle&) = delete;

            /*! Prohibit copy constructor
             */
            Circle(Circle&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const Circle&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(Circle&) = delete;

        protected:
            // --- IElement ---
            virtual std::unique_ptr<XML::Element> toXML() const
            {
                std::unique_ptr<XML::Element> element = IElement::toXML();
                element.get()->setName("circle");
                element.get()->setAttribute("cx", m_cx);
                element.get()->setAttribute("cy", m_cy);
                element.get()->setAttribute("r", m_r);
                return element;
            }

        private:
            // --- Members ---
            int m_cx;      //!< The x-axis center of the circle
            int m_cy;      //!< The y-axis center of the circle
            int m_r;       //!< The radius of the circle
    };

}

#endif // CIRCLe_HPP
