#pragma once
#ifndef GRAPHICS_SVGGRAPHICS_HPP
#define GRAPHICS_SVGGRAPHICS_HPP

#include "igraphics.hpp"

namespace graphics
{
    class SVGGraphics : public IGraphics
    {
        public:
            /*! Default constructor
             */
            explicit SVGGraphics() = default;

            /*! Default destructor
             */
            virtual ~SVGGraphics() = default;

            /*! Prohibit const copy constructor
             */
            SVGGraphics(const SVGGraphics&) = delete;

            /*! Prohibit copy constructor
             */
            SVGGraphics(SVGGraphics&) = delete;

            virtual void drawLine(int x1, int y1, int x2, int y2)
            {

            }

            virtual void drawOval(int x, int y, int width, int height)
            {

            }

            virtual void drawCircle(int x, int y, int radius)
            {
                drawOval(x, y, radius, radius);
            }

            virtual void fillOval(int x, int y, int width, int height)
            {

            }
            virtual void fillCircle(int x, int y, int radius)
            {
                fillOval(x, y, radius, radius);
            }

            virtual void drawPolygon(int[] xs, int[] ys)
            {

            }
            virtual void drawPolyline(int[] xs, int[] ys)
            {

            }

            virtual void fillPolygon(int[] xs, int[] ys)
            {

            }

            virtual void drawRect(int x, int y, int width, int height)
            {

            }
            virtual void drawRoundRect(int x, int y, int width, int height, int arcWidth, int arcHeight)
            {

            }

            virtual void fillRect(int x, int y, int width, int height)
            {

            }
            virtual void fillRoundRect(int x, int y, int width, int height, int arcWidth, int arcHeight)
            {

            }

            virtual void drawString(int x, int y, std::string text)
            {

            }

            virtual void setStrokeColor(int r, int g, int b, int alpha)
            {

            }
            virtual void setFillColor(int r, int g, int b, int alpha)
            {

            }

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // GRAPHICS_SVGGRAPHICS_HPP