#pragma once
#ifndef SVG_POLYGON_HPP
#define SVG_POLYGON_HPP

#include "svg/ielement.hpp"

namespace SVG
{

    /*! Defines a Polygon
     */
    class Polygon final : public IElement
    {
        public:
            /*! Constructor
            	 \param[in] points	the points making up the Polygon
             */
            explicit Polygon(const std::vector<std::pair<double,double>>& points)
            {
                for(auto point : points)
                {
                    m_points.push_back(point);
                }
            }

            /*! Default destructor
             */
            virtual ~Polygon() = default;

            /*! Prohibit const copy constructor
             */
            Polygon(const Polygon&) = delete;

            /*! Prohibit copy constructor
             */
            Polygon(Polygon&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const Polygon&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(Polygon&) = delete;

        protected:
            // ---  IElement ---
            virtual std::unique_ptr<XML::IElement> toXML() const
            {
                std::ostringstream os;
                for(int i=0; i<m_points.size(); i++)
                {
                    os << ( i == 0 ? "" : " ") << m_points[i].first << "," << m_points[i].second;
                }
                auto node = IElement::toXML();
                node.get()->setName("polygon");
                node.get()->setAttribute("points", os.str());
                return node;
            }

        private:
            // --- Members ---
            std::vector<std::pair<double,double>> m_points;
    };

}

#endif // SVG_POLYGON_HPP
