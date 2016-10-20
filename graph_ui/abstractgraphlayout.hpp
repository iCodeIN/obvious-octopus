#pragma once
#ifndef ABSTRACTGRAPHLAYOUT_HPP
#define ABSTRACTGRAPHLAYOUT_HPP

#include <assert.h>
#include <functional>
#include <tuple>
#include <vector>

namespace graph
{
    template <typename T> class IGraph;
}

namespace graphUI
{
    /*! Abstract base class for laying out graphs
     */
    template <typename T>
    class AbstractGraphLayout
    {
        public:
            explicit AbstractGraphLayout() = default;
            virtual ~AbstractGraphLayout() = default;

            /*! define a phyiscal point
             */
            using PointType = typename std::tuple<int,int>;

            /*! define a function mapping vertices to points
             */
            using VertexLayoutFunctionType = typename std::function<PointType(const T &vertex)>;

            /*! define a function mapping edges to the points they cross
             */
            using EdgeLayoutFunctionType   = typename std::function<std::vector<PointType>(const T &source, const T &target)>;

            /*! define a layout as a combination of VertexLayoutFunctionType and EdgeLayoutFunctionType
             */
            using LayoutType = typename std::tuple<VertexLayoutFunctionType, EdgeLayoutFunctionType>;

            /*! define a size (bounding box) type
             */
            using SizeType = typename std::tuple<int,int>;

            /*! define a function mapping each vertex to its size
                This is useful when laying out the graph, so that the size of vertices can be taken into account.
             */
            using SizeFunctionType = typename std::function<SizeType(const T &vertex)>;

            /*! Layout a graph
             */
            virtual LayoutType layout(const graph::IGraph<T> &graph, const SizeFunctionType &size) = 0;

            /*! set the minimal margin on the x-axis between vertices
             */
            virtual void setMinXMargin(int margin)
            {
                assert(margin >=0 && margin <= m_maxXMargin);
                m_minXMargin = margin;
            }

            /*! \return the minimal margin on the x-axis between vertices
             */
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

            /*! \return the maximal margin on the x-axis between vertices
             */
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

            /*! \return the minimal margin on the y-axis between vertices
             */
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

            /*! \return the maximal margin on the y-axis between vertices
             */
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
