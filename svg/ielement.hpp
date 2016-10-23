#pragma once
#ifndef SVG_IELEMENT_HPP
#define SVG_IELEMENT_HPP

#include "xml/boostelementimpl.hpp"

#include <math.h>
#include <memory>
#include <sstream>

namespace SVG
{
    /*! Common abstract base class for SVG elements
     */
    class IElement
    {
        public:
            /*! constructor
             */
            explicit IElement() = default;

            /*! destructor
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

            // --- Methods ---

            /*! Sets the stroke-width property.
            	 The stroke-width property defines the thickness of a line, text or outline of an element.
            	 Its default value is 1.
             */
            void setStrokeWidth(double w)
            {
                m_strokeWidth = w;
            }
            /*! Sets the stroke-width property to 1.
             */
            void clearStrokeWidth()
            {
                setStrokeWidth(1);
            }

            /*! Sets the stroke property.
            	 The stroke property defines the color of a line, text or outline of an element.
            	 Its default value is (0, 0, 0).
             */
            void setStroke(int r, int g, int b)
            {
                m_stroke = std::make_tuple(r,g,b);
            }
            /*! Sets the stroke property to (0, 0, 0).
             */
            void clearStroke()
            {
                setStroke(0,0,0);
            }

            /*! Sets the stroke-opacity property.
            	 The stroke-opacity property specifies the opacity of the outline on the current object.
            	 Its default value is 1.
             */
            void setStrokeOpacity(double alpha)
            {
                m_strokeOpacity = alpha;
            }
            /*! Sets the stroke-opacity property to 1.
             */
            void clearStrokeOpacity()
            {
                setStrokeOpacity(1.0);
            }

            /*! Sets the fill property.
            	 For shapes and text, the fill attribute is a presentation attribute that define the color of the interior of the given graphical element.
            	 What is called the "interior" depends on the shape itself and the value of the fill-rule attribute.
            	 Its default value is (255, 255, 255).
             */
            void setFill(int r,int g, int b)
            {
                m_fill = std::make_tuple(r,g,b);
            }
            /*! Sets the fill property to (255, 255, 255).
             */
            void clearFill()
            {
                setFill(255,255,255);
            }

            /*! Sets the fill-opacity property.
            	 This property specifies the opacity of the color or the content the current object is filled with.
            	 Its default value is 0.
             */
            void setFillOpacity(double alpha)
            {
                m_fillOpacity = alpha;
            }
            /*! Sets the fill-opacity property to 0.
             */
            void clearFillOpacity()
            {
                setFillOpacity(0.0);
            }

            /*! This transform definition specifies a transformation in the form of a transformation matrix of six values.
                Matrix(t0, t1, t2, t3, t4, t5) is equivalent to applying the transformation matrix
               \f$
                  \begin{pmatrix}
                  t_{0} & t_{2}  & t_{4} \\
                  t_{1} & t_{3}  & t_{5} \\
                  0     & 0      & 1     \\
                  \end{pmatrix}
               \f$
             */
            void matrix(double t0, double t1, double t2, double t3, double t4, double t5)
            {
                std::tuple<double, double, double, double, double, double> t = std::make_tuple(t0, t1, t2, t3, t4, t5);
                m_transform.push_back(t);
            }

            /*! This transform definition specifies a translation by x and y. This is equivalent to matrix(1 0 0 1 x y).
             */
            void translate(double x, double y)
            {
                matrix(1, 0, 0, 1, x, y);
            }
            /*! This transform definition specifies a scale operation by x and y. This is equivalent to matrix(x 0 0 y 0 0).
             */
            void scale(double x, double y)
            {
                matrix(x, 0, 0, y, 0, 0);
            }

            /*! This transform definition specifies a rotation by a degrees about a given point. The rotate is about the origin of the current user coordinate system.
                The operation corresponds to the matrix
               \f$
                  \begin{pmatrix}
                  cos(a) & -sin(a)  & 0 \\
                  sin(a) & cos(a)   & 0 \\
                  0      & 0        & 1 \\
                  \end{pmatrix}
               \f$
             */
            void rotate(double a)
            {
                matrix(cos(a), sin(a), -sin(a), cos(a), 0, 0);
            }

            /*! This transform definition specifies a rotation by a degrees about a given point.
                The rotate is about the point (x, y). The operation represents the equivalent of the following transform definitions list: translate(x, y) rotate(a) translate(-x, -y).
             */
            void rotate(double a, double x, double y)
            {
                translate(x, y);
                rotate(a);
                translate(-x, -y);
            }

            /*! This transform definition specifies a skew transformation along the x axis by a degrees.
                The operation corresponds to the matrix
               \f$
                  \begin{pmatrix}
                  1 & tan(a)  & 0 \\
                  0 & 1       & 0 \\
                  0 & 0       & 1 \\
                  \end{pmatrix}
               \f$
             */
            void skewX(double a)
            {
                matrix(1, 0, tan(a), 1, 0, 0);
            }

            /*! This transform definition specifies a skew transformation along the y axis by a degrees.
                The operation corresponds to the matrix
               \f$
                  \begin{pmatrix}
                  1      & 0  & 0 \\
                  tan(a) & 1  & 0 \\
                  0      & 0  & 1 \\
                  \end{pmatrix}
               \f$
             */
            void skewY(double a)
            {
                matrix(1, tan(a), 0, 1, 0, 0);
            }

            /*! Writes an IElement to an std::ostream
             	 \param[in] os	std::ostream to write the IElement to
            	 \param[in] e	IElement to stream
             */
            friend std::ostream& operator<<(std::ostream &os, const IElement &e)
            {
                auto node = e.toXML();
                assert(node.get());
                os << node.get();
                return os;
            }

            /*! \return the XML element representing this SVG element
             */
            virtual std::unique_ptr<XML::IElement> toXML() const
            {
                auto node = std::unique_ptr<XML::IElement>(new XML::BoostElementImpl(""));

                std::ostringstream osStyle;
                osStyle << "fill:rgb(" << std::get<0>(m_fill) << "," << std::get<1>(m_fill) << "," << std::get<2>(m_fill) << ");";
                osStyle << "fill-opacity:" << m_fillOpacity << ";";
                osStyle << "stroke:rgb(" << std::get<0>(m_stroke) << "," << std::get<1>(m_stroke) << "," << std::get<2>(m_stroke) << ");";
                osStyle << "stroke-opacity:" << m_strokeOpacity << ";";
                osStyle << "stroke-width:" << m_strokeWidth;
                node->setAttribute("style", osStyle.str());

                if(!m_transform.empty())
                {
                    std::ostringstream osTransform;
                    for(int i=0; i<m_transform.size(); i++)
                    {
                        const auto &t = m_transform[i];
                        osTransform << (i==0?"":" ") << "matrix(" << std::get<0>(t) << " " << std::get<1>(t) << " " << std::get<2>(t) << " " << std::get<3>(t) << " " << std::get<4>(t) << " " << std::get<5>(t) << ")";
                    }
                    node->setAttribute("transform", osTransform.str());
                }

                return std::move(node);
            }

        private:

            // --- Members ---
            using ColorType = std::tuple<int,int,int>;
            double      m_strokeWidth     = 2;                                         //!< stroke-width property
            ColorType   m_stroke          = std::make_tuple(0,0,0);                    //!< stroke property
            double      m_strokeOpacity   = 1.0;                                       //!< stroke-opacity property
            ColorType   m_fill            = std::make_tuple(255,255,255);              //!< fill property
            double      m_fillOpacity     = 0.0;                                       //!< fill-opacity property

            using TransformType = std::tuple<double, double, double, double, double, double>;
            std::vector<TransformType> m_transform;

    };

} // end of namespace SVG

#endif // SVG_IELEMENT_HPP
