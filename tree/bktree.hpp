#pragma once
#ifndef BKTREE_HPP
#define BKTREE_HPP

#include <functional>
#include <map>
#include <memory>

namespace tree
{
    /*! A BK-tree is a metric tree suggested by Walter Austin Burkhard and Robert M. Keller specifically adapted to discrete metric spaces.
        For simplicity, let us consider integer discrete metric d(x , y). Then, BK-tree is defined in the following way.
        An arbitrary element a is selected as root node. The root node may have zero or more subtrees.
        The k-th subtree is recursively built of all elements b such that d(a , b) = k.
        BK-trees can be used for approximate string matching in a dictionary.
     */
    template <typename T>
    class BKTree final
    {
        public:
            /*! Define a metric for objects of type T
             */
            using MetricType = std::function<int(const T &t0, const T &t1)>;

            /*! constructor (default)
                \param[in] metric   the metric used for constructing this BKTree
             */
            explicit BKTree(const MetricType &metric)
                : m_metric(metric)
            {
            }

            /*! destructor (default)
             */
            virtual ~BKTree() = default;

            /*! insert data into this BKTree
                \return true if the BKTree changed due to the insert operation,
                        false otherwise
             */
            bool insert(const T &data)
            {
                if(m_root.get() == nullptr)
                {
                    m_root = std::unique_ptr<BKTreeNode<T>>(new BKTreeNode<T>(*this, data));
                    return true;
                }
                else
                {
                    return m_root.get()->insert(data);
                }
            }

            /*!
             */
            std::vector<T> find(const T &data, int diff) const
            {

            }

            /*! \return the depth of this BKTree
             */
            int depth() const
            {
                if(m_root = nullptr || m_root.get() == nullptr)
                {
                    return 0;
                }
                return m_root->depth();
            }

        private:
            // --- methods ---

            // --- classes ---
            template <typename U> class BKTreeNode; // forward declare to enable children
            class BKTreeNode<T>
            {
                public:
                    /*! constructor (default)
                        \param[in] tree     the tree to which this node belongs
                        \param[in] data     the data in this node
                     */
                    explicit BKTreeNode(BKTree<T> &tree, const T &data)
                        : m_tree(tree)
                        , m_data(data)
                    {
                    }

                    /*! insert data at this node (may generate a new node)
                     */
                    bool insert(const T &data)
                    {
                        auto d = m_tree.m_metric(data, m_data);
                        if(d == 0)
                        {
                            return false;
                        }
                        if(m_children.find(d) != m_children.end())
                        {
                            m_children[d] = std::unique_ptr<BKTreeNode<T>>(new BKTreeNode<T>(m_tree, data));
                            return true;
                        }
                        else
                        {
                            return m_children.find(d)->second.get()->insert(data);
                        }
                    }

                    int depth() const
                    {
                        if(m_children.empty())
                        {
                            return 0;
                        }
                        auto maxDepth = 0;
                        for(auto &pair : m_children)
                        {
                            auto d = pair->second.depth();
                            maxDepth = d > maxDepth ? d : maxDepth;
                        }
                        return maxDepth;
                    }

                private:
                    T                                               m_data;         //!< the data in this node
                    BKTree<T>                                      &m_tree;         //!< the tree to which this node belongs
                    std::map<int, std::unique_ptr<BKTreeNode<T>>    m_children;     //!< the children of this node
            };

            // --- members ---
            const MetricType                m_metric;   //!< the metric used when constructing the tree
            std::unique_ptr<BKTreeNode<T>>  m_root;     //!< the root-node of the tree
    };
}

#endif // BKTREE_HPP