#ifndef AREAPLOT_H
#define AREAPLOT_H

#include <functional>
#include <string>
#include <vector>
#include <memory>

namespace SVG
{
    class SVG;
}

namespace plotting
{
    /*! Area charts are used to represent cumulated totals using numbers or percentages (stacked area charts in this case) over time.
    	 Use the area chart for showing trends over time among related attributes.
    	 The area chart is like the plot chart except that the area below the plotted line is filled in with color to indicate volume.
    	 When multiple attributes are included, the first attribute is plotted as a line with color fill followed by the second attribute, and so on.
     */
    class AreaPlot final
    {
        public:
            /*! Constructor
            	 \param[in] w	width of the AreaPlot
            	 \param[in] h	height of the AreaPlot
             */
            explicit AreaPlot(int w, int h);

            /*! Default destructor
             */
            virtual ~AreaPlot() = default;

            /*! Prohibit copy constructor
             */
            AreaPlot(AreaPlot &n) = delete;

            /*! Prohibit const copy constructor
             */
            AreaPlot(const AreaPlot &n) = delete;

            /*! Prohibit assignment operator
             */
            AreaPlot operator=(AreaPlot &&) = delete;

            /*! Prohibit const assignment operator
             */
            AreaPlot operator=(const AreaPlot &&) = delete;

            /*! Define a Series
             */
            using SeriesType = std::vector<std::pair<double,double>>;

            /*! Add a series of x/y-values to this AreaPlot
             */
            void addSeries(const SeriesType& s);

            /*! Render a AreaPlot as an SVG image
             */
            std::unique_ptr<SVG::SVG> render();

        private:
            // --- Members ---
            std::vector<std::reference_wrapper<const SeriesType>> 		m_series;      //!< All series to be included in this plot
            int                  													m_width;       //!< Output width
            int                  													m_height;      //!< Output height
    };
}

#endif // AREAPLOT_H
