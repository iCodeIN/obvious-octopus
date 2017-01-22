#pragma once
#ifndef ABSTRACTGRAPHLAYOUT_HPP
#define ABSTRACTGRAPHLAYOUT_HPP

#include "graph_ui/igraphlayout.h"

#include <assert.h>
#include <functional>
#include <tuple>
#include <vector>

namespace graph
{
    template <typename T> class IGraph;
}

namespace graph_ui
{
    /*! Abstract base class for laying out graphs
     */
    template <typename T>
    class AbstractGraphLayout : public IGraphLayout<T>
    {
        public:
            /*! default constructor
             */
            explicit AbstractGraphLayout() = default;

            /*! default destructor
             */
            virtual ~AbstractGraphLayout() = default;

            /*! define a phyiscal point
             */
            using PointType = typename std::tuple<int,int>;

            /*! define a size (bounding box) type
             */
            using SizeType = typename std::tuple<int,int>;

            /*! define a function mapping each vertex to its size
                This is useful when laying out the graph, so that the size of vertices can be taken into account.
             */
            using SizeFunctionType = typename std::function<SizeType(const T &vertex)>;

            //! --- IGraphLayout ---
            virtual std::unique_ptr<graph::I2DGraph<T>> layout(const graph::IGraph<T> &graph, const SizeFunctionType &size) const = 0;

            /*! set the minimal margin on the x-axis between vertices
             */
            virtual void setMinXMargin(int margin)
            {
                assert(margin >=0 && margin <= m_maxXMargin);
                m_minXMargin = margin;
            }

            // --- IGraphLayout ---
            virtual int getMinXMargin() const
            {
                return m_minXMargin;
            }

            /*! set the maximal margin on the x-axis between vertices
             */
            virtual void setMaxXMargin(int margin)
            {
                assert(margin >=0 && margin >= m_minXMargin);
                m_maxXMargin = margin;
            }

            // --- IGraphLayout ---
            virtual int getMaxXMargin() const
            {
                return m_maxXMargin;
            }

            /*! set the minimal margin on the y-axis between vertices
             */
            virtual void setMinYMargin(int margin)
            {
                assert(margin >= 0 && margin <= m_maxYMargin);
                m_minYMargin = margin;
            }

            // --- IGraphLayout ---
            virtual int getMinYMargin() const
            {
                return m_minYMargin;
            }

            /*! set the maximal margin on the y-axis between vertices
             */
            virtual void setMaxYMargin(int margin)
            {
                assert(margin >= 0 && margin >= m_minYMargin);
                m_maxYMargin = margin;
            }

            // --- IGraphLayout ---
            virtual int getMaxYMargin() const
            {
                return m_maxYMargin;
            }

        private:
            // --- members ---
            int     m_minXMargin = 10;
            int     m_maxXMargin = 10;
            int     m_minYMargin = 10;
            int     m_maxYMargin = 10;

    };
}

#endif // ABSTRACTGRAPHLAYOUT_HPP
