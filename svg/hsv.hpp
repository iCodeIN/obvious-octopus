#pragma once
#ifndef HSV_HPP
#define HSV_HPP

#include <assert.h>
#include <tuple>
#include <vector>

namespace SVG
{
    /*! HSV is one of the most common cylindrical-coordinate representations of points in an RGB color model.
    	 The two representations rearrange the geometry of RGB in an attempt to be more intuitive and perceptually
    	 relevant than the cartesian (cube) representation. Developed in the 1970s for computer graphics applications,
    	 HSV is used today in color pickers, in image editing software, and less commonly in image analysis and computer vision.
    	 HSV stands for hue, saturation, and value, and is also often called HSB (hue, saturation, brightness)
     */
    class HSV final
    {
        public:
            /*! Prohibit construction of HSV. This class offers only static methods.
             */
            explicit HSV() = delete;

            /*! Define a HSV-color.
             */
            using HSVColor = std::tuple<double,double,double>;

            /*! Define an RGB-color.
             */
            using RGBColor = std::tuple<int,int,int>;

            /*! Convert a color from the HSV space to the RGB space
             */
            static RGBColor toRGB(HSVColor hsv)
            {
                double h = std::get<0>(hsv);
                double s = std::get<1>(hsv);
                double v = std::get<2>(hsv);

                assert(h>=0 && h<=360 && s>=0 && s<=1.0 && v>=0 && v<=1.0);

                auto C = v * s;
                auto H = (int) h / 60;
                auto X = C * (1.0 - abs(H % 2 - 1.0));

                double r = 0;
                double g = 0;
                double b = 0;
                if(H < 1.0)
                {
                    r = C;
                    g = X;
                }
                else if(H < 2.0)
                {
                    r = X;
                    g = C;
                }
                else if(H < 3.0)
                {
                    g = C;
                    b = X;
                }
                else if(H < 4.0)
                {
                    g = X;
                    b = C;
                }
                else if(H < 5.0)
                {
                    r = X;
                    b = C;
                }
                else if(H <= 6.0)
                {
                    r = C;
                    b = X;
                }

                auto min = v - C;
                r += min;
                g += min;
                b += min;

                auto retVal = std::make_tuple(r*255, g*255, b*255);
                return retVal;
            }

            /*! Generate n equidistant HSV colors
             */
            static std::vector<HSVColor> equidistant(int n)
            {
                auto colors = std::vector<HSV::HSVColor>();
                for(int i=0; i<n; i++)
                {
                    double h = ((360 / n) * i + 45) % 360;
                    double s = 1.0;
                    double v = 1.0;
                    colors.push_back(std::make_tuple(h,s,v));
                }
                return colors;
            }

    };
}

#endif // HSV_HPP
