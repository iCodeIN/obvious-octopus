#pragma once
#ifndef GRAPHICS_IGRAPHICS_HPP
#define GRAPHICS_IGRAPHICS_HPP

namespace graphics
{
    class IGraphics
    {
        public:
            /*! Default constructor
             */
            explicit IGraphics() = default;

            /*! Default destructor
             */
            virtual ~IGraphics() = default;

            /*! Prohibit const copy constructor
             */
            IGraphics(const IGraphics&) = delete;

            /*! Prohibit copy constructor
             */
            IGraphics(SVG&) = delete;

            virtual void drawLine(int x1, int y1, int x2, int y2) = 0;

            virtual void drawOval(int x, int y, int width, int height) = 0;
            virtual void drawCircle(int x, int y, int radius) = 0;

            virtual void fillOval(int x, int y, int width, int height) = 0;
            virtual void fillCircle(int x, int y, int radius) = 0;

            virtual void drawPolygon(int[] xs, int[] ys) = 0;
            virtual void drawPolyline(int[] xs, int[] ys) = 0;

            virtual void fillPolygon(int[] xs, int[] ys) = 0;

            virtual void drawRect(int x, int y, int width, int height) = 0;
            virtual void drawRoundRect(int x, int y, int width, int height, int arcWidth, int arcHeight) = 0;

            virtual void fillRect(int x, int y, int width, int height) = 0;
            virtual void fillRoundRect(int x, int y, int width, int height, int arcWidth, int arcHeight) = 0;

            virtual void drawString(int x, int y, std::string text) = 0;

            virtual void setStrokeColor(int r, int g, int b, int alpha) = 0;
            virtual void setFillColor(int r, int g, int b, int alpha) = 0;

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // GRAPHICS_IGRAPHICS_HPP