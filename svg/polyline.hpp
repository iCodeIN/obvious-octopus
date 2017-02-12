#pragma once
#ifndef SVG_POLYLINE_HPP
#define SVG_POLYLINE_HPP

#include "svg/ielement.hpp"

namespace SVG
{

    /*! Defines a Polyline
     */
    class Polyline final : public IElement
    {
        public:
            // --- Methods ---
            /*! Constructor
            	 \param[in] points	the points making up the Polyline
             */
            explicit Polyline(const std::vector<std::pair<double,double>>& points)
            {
                for(auto point : points)
                {
                    m_points.push_back(point);
                }
            }

            /*! Default destructor
             */
            virtual ~Polyline() = default;

            /*! Prohibit const copy constructor
             */
            Polyline(const Polyline&) = delete;

            /*! Prohibit copy constructor
             */
            Polyline(Polyline&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const Polyline&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(Polyline&) = delete;

        protected:
            // -- IElement ---
            virtual std::unique_ptr<XML::IElement> toXML() const
            {
                std::ostringstream os;
                for(int i=0; i<m_points.size(); i++)
                {
                    os << ( i == 0 ? "" : " ") << m_points[i].first << "," << m_points[i].second;
                }
                auto node = IElement::toXML();
                node.get()->setName("polyline");
                node.get()->setAttribute("points", os.str());
                return node;
            }

        private:
            // --- methods ---
            // --- members ---
            std::vector<std::pair<double,double>> m_points;
    };

}

#endif // SVG_POLYLINE_HPP
