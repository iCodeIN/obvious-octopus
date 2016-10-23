#pragma once
#ifndef SVG_HPP
#define SVG_HPP

#include "svg/ielement.hpp"

#include <sstream>

namespace SVG
{
    /*! Scalable Vector Graphics (SVG) is an XML-based vector image format for two-dimensional graphics with support for interactivity and animation.
    	 The SVG specification is an open standard developed by the World Wide Web Consortium (W3C) since 1999.
    	 SVG images and their behaviors are defined in XML text files. This means that they can be searched, indexed, scripted, and compressed.
     	 As XML files, SVG images can be created and edited with any text editor, but are more often created with drawing software.
    	 All major modern web browsers—including Mozilla Firefox, Internet Explorer, Google Chrome, Opera, and Safari—have at least
    	 some degree of SVG rendering support.
     */
    class SVG final : public IElement
    {
        public:
            /*! Default constructor
             */
            explicit SVG() = default;

            /*! Default destructor
             */
            virtual ~SVG() = default;

            /*! Prohibit const copy constructor
             */
            SVG(const SVG&) = delete;

            /*! Prohibit copy constructor
             */
            SVG(SVG&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const SVG&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(SVG&) = delete;

            /*! Add an IElement to this SVG.
             */
            void add(std::unique_ptr<IElement> e)
            {
                m_elements.push_back(std::move(e));
            }

            /*! Set the width of this SVG
             */
            void setWidth(int w)
            {
                m_width = w;
            }
            /*! Set the height of this SVG
             */
            void setHeight(int h)
            {
                m_height = h;
            }

            /*! This transform definition specifies a reversal along the central vertical axis.
                The operation represents the equivalent of the following transform definitions list: scale(1, -1) translate(0, height)
             */
            void flipHorizontal()
            {
                scale(1, -1);
                translate(0, -m_height);
            }

            /*! This transform definition specifies a reversal along the central horizontal axis.
                The operation represents the equivalent of the following transform definitions list: scale(-1, 1) translate(width, 0)
             */
            void flipVertical()
            {
                scale(-1, 1);
                translate(m_width, 0);
            }

            /*! Set the ViewBox property of this SVG.
            	 The viewBox property specifies that a given set of graphics stretch to fit a particular container element.
            	 \param[in] minX		x-value of the point at the upper-left corner of the ViewBox
            	 \param[in] minY		y-value of the point at the upper-left corner of the ViewBox
            	 \param[in] width		width of the ViewBox
            	 \param[in] height	height of the ViewBox
             */
            void setViewBox(int minX, int minY, int width, int height)
            {
                m_viewbox_minX = minX;
                m_viewbox_minY = minY;
                m_viewbox_width = width;
                m_viewbox_height = height;
            }

            /*! Clear the ViewBox property of this SVG.
             */
            void clearViewBox()
            {
                m_viewbox_minX = -1;
                m_viewbox_minY = -1;
                m_viewbox_width = -1;
                m_viewbox_height = -1;
            }

            /*! In some cases, typically when using the viewBox attribute, it is desirable that the graphics stretch to fit non-uniformly to take up the entire viewport.
                In other cases, it is desirable that uniform scaling be used for the purposes of preserving the aspect ratio of the graphics.
                Attribute preserveAspectRatio indicates whether or not to force uniform scaling.
                For all elements that support that attribute, except for the &lt;image&gt; element, preserveAspectRatio only applies when a value has been provided for viewBox on the same element.
                For these elements, if attribute viewBox is not provided, then preserveAspectRatio is ignored.
             */
            enum PreserveAspectRatio
            {
                NONE,       //!< Do not force uniform scaling. Scale the graphic content of the given element non-uniformly if necessary such that the element's bounding box exactly matches the viewport rectangle.

                XMINYMIN,   //!< Align the min-x of the element's viewBox with the smallest X value of the viewport. Align the min-y of the element's viewBox with the smallest Y value of the viewport.
                XMIDYMIN,   //!< Align the midpoint X value of the element's viewBox with the midpoint X value of the viewport. Align the min-y of the element's viewBox with the smallest Y value of the viewport.
                XMAXYMIN,   //!< Align the min-x+width of the element's viewBox with the maximum X value of the viewport. Align the min-y of the element's viewBox with the smallest Y value of the viewport.

                XMINYMID,   //!< Align the min-x of the element's viewBox with the smallest X value of the viewport. Align the midpoint Y value of the element's viewBox with the midpoint Y value of the viewport.
                XMIDYMID,   //!< Align the midpoint X value of the element's viewBox with the midpoint X value of the viewport. Align the midpoint Y value of the element's viewBox with the midpoint Y value of the viewport.
                XMAXYMID,   //!< Align the min-x+width of the element's viewBox with the maximum X value of the viewport. Align the midpoint Y value of the element's viewBox with the midpoint Y value of the viewport.

                XMINYMAX,   //!< Align the min-x of the element's viewBox with the smallest X value of the viewport. Align the min-y+height of the element's viewBox with the maximum Y value of the viewport.
                XMIDYMAX,   //!< Align the midpoint X value of the element's viewBox with the midpoint X value of the viewport. Align the min-y+height of the element's viewBox with the maximum Y value of the viewport.
                XMAXYMAX    //!< Align the min-x+width of the element's viewBox with the maximum X value of the viewport. Align the min-y+height of the element's viewBox with the maximum Y value of the viewport.
            };

            /*! Set the PreserveAspectRatio attribute of this SVG.
             */
            void setPreserveAspectRatio(SVG::PreserveAspectRatio &aspectRatio)
            {
                m_preserveAspectRatio = aspectRatio;
            }

            /*! Clear the PreserveAspectRatio attribute of this SVG.
             */
            void clearPreserveAspectRatio()
            {
                m_preserveAspectRatio = PreserveAspectRatio::NONE;
            }

            /*! \return the XML element representing this SVG image
             */
            virtual std::unique_ptr<XML::IElement> toXML() const
            {
                std::unique_ptr<XML::IElement> node = SVG::IElement::toXML();
                node.get()->setName("svg");

                // set width
                if(m_width>=0)
                {
                    node.get()->setAttribute("width", m_width);
                }

                // set height
                if(m_height>=0)
                {
                    node.get()->setAttribute("height", m_height);
                }

                // set viewbox
                if(m_viewbox_minX >= 0 && m_viewbox_minY >= 0 && m_viewbox_width >= 0 && m_viewbox_height >= 0)
                {
                    std::ostringstream osViewBox;
                    osViewBox << m_viewbox_minX << " " << m_viewbox_minY << " " << m_viewbox_width << " " << m_viewbox_height;
                    node.get()->setAttribute("viewBox", osViewBox.str());

                    const std::string aspectRatioToString[] = {"none", "xMinYMin", "xMidYMin","xMaxYMin", "xMinYMid", "xMidYMid", "xMaxYMid", "xMinYMax", "xMidYMax", "xMaxYMax"};
                    node.get()->setAttribute("preserveAspectRatio", aspectRatioToString[m_preserveAspectRatio]);
                }

                // add children
                for(auto &element : m_elements)
                {
                    node.get()->add(element.get()->toXML());
                }
                int i = 0;
                while(node.get()->hasChild(i))
                {
                    i++;
                }
                return node;
            }

        private:
            // --- Members ---
            std::vector<std::unique_ptr<IElement>> m_elements;

            PreserveAspectRatio                    m_preserveAspectRatio   = PreserveAspectRatio::NONE;  //!< PreserveAspectRatio attribute
            int												m_viewbox_minX 	      = -1;		                     //!< ViewBox upper-left corner X
            int 												m_viewbox_minY 	      = -1;		                     //!< ViewBox upper-left corner Y
            int 												m_viewbox_width 	      = -1;		                     //!< ViewBox width property
            int 												m_viewbox_height 	      = -1;		                     //!< ViewBox height property

            int 												m_width 				      = -1;		                     //!< width
            int 												m_height				      = -1;		                     //!< height
    };
}

#endif // SVG_HPP
