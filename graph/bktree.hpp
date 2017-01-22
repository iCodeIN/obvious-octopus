#ifndef BKTREE_HPP
#define BKTREE_HPP

#include "graph/adjecencylisttree.hpp"

namespace graph
{
    /*! represents a node in a BKTree
     */
    template <typename T>
    class BKTreeNode
    {
        public:
            /*! constructor
             */
            BKTreeNode(const T& data, int dist)
                : m_data(data)
                , m_dist(dist)
            {
            }

            virtual ~BKTreeNode() = default;

            /*! \return the distance between this node and its parent
             */
            int getDist() const
            {
                return m_dist;
            }
            /*! \return the data in this BKTreeNode
             */
            const T& getData() const
            {
                return m_data;
            }
        private:
            // --- methods ---
            // --- members ---
            T   m_data;
            int m_dist;
    };

    /*! A BK-tree is a metric tree suggested by Walter Austin Burkhard and Robert M. Keller specifically adapted to discrete metric spaces.
        For simplicity, let us consider integer discrete metric d(x, y). Then, BK-tree is defined in the following way;
        An arbitrary element a is selected as root node.
        The root node may have zero or more subtrees.
        The k-th subtree is recursively built of all elements b such that d(a, b) = k.
        BK-trees can be used for approximate string matching in a dictionary
     */
    template <typename T>
    class BKTree : public AdjecencyListTree<BKTreeNode<T>*>
    {
        public:
            /*! define a metric over T
                i.e. a function that takes two const references of type T and outputs the 'distance' (a positive, int-valued number) between them
             */
            using MetricFunctionType = std::function<int(const T& t0, const T& t1)>;

            /*! constructor
             */
            explicit BKTree(const MetricFunctionType& metric)
                : m_metric(metric)
            {
            }

            /* default destructor
             */
            virtual ~BKTree()
            {
                for(auto &vertex : this->vertices())
                {
                    delete vertex;
                }
            }

            /*! Insert a new T in this BKTree
                \return true iff the tree was modified as a result of this operation, false otherwise
             */
            bool insert(const T& t)
            {
                // special case for root
                if(this->vertices().empty())
                {
                    this->insertVertex(new BKTreeNode<T>(t, 0));
                    return true;
                }
                // general case
                else
                {
                    auto nodePtr = this->root();
                    while(true)
                    {
                        auto dist = m_metric(nodePtr->getData(), t);
                        if(dist == 0)
                        {
                            return false;
                        }
                        auto childFound = false;
                        for(auto next : this->outgoing(nodePtr))
                        {
                            if(next->getDist() == dist)
                            {
                                nodePtr = next;
                                childFound = true;
                                break;
                            }
                        }
                        if(!childFound)
                        {
                            auto newNodePtr = new BKTreeNode<T>(t, dist);
                            this->insertVertex(newNodePtr);
                            this->insertEdge(nodePtr, newNodePtr);
                            return true;
                        }
                    }
                }
            }

            /*! \return an std::vector containing all Ts that differ only the given distance from the given T
             */
            std::vector<T> find(const T& t, int maxDistance) const
            {
                std::vector<T> retval;
                // empty tree
                if(AdjecencyListTree<BKTreeNode<T>*>::vertices().empty())
                {
                    return retval;
                }
                else
                {
                    findRecursive(this->root(), t, maxDistance, retval);
                }
                // return
                return retval;
            }

        private:
            // --- methods ---
            void findRecursive(const BKTreeNode<T>* nodePtr, const T& t, int maxDist, std::vector<T>& retval) const
            {
                auto dist = m_metric(nodePtr->getData(), t);
                if(dist <= maxDist)
                {
                    retval.push_back(nodePtr->getData());
                }
                auto lowerLimit = std::max(0, dist - maxDist);
                auto upperLimit = dist + maxDist;
                for(auto &next : this->outgoing(const_cast<BKTreeNode<T>*>(nodePtr)))
                {
                    if(next->getDist() >= lowerLimit && next->getDist() <= upperLimit)
                    {
                        findRecursive(next, t, maxDist, retval);
                    }
                }
            }
            // --- members ---
            MetricFunctionType m_metric;
    };
}

#endif // BKTREE_HPP