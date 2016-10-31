#ifndef BOOST_MATRIX_IMPL_HPP
#define BOOST_MATRIX_IMPL_HPP

#include "imatrix.hpp"

#include <assert.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <random>

namespace matrix
{
    /*! Implementation of IMatrix using boost matrix type
     */
    class BoostMatrixImpl : public IMatrix
    {
        public:
            /*! constructor
                \param[in] rows number of rows to allocate
                \param[in] cols number of columns to allocate
             */
            explicit BoostMatrixImpl(int rows, int cols)
                : m_nofCols(cols)
                , m_nofRows(rows)
                , m_data(rows, cols)
            {
                assert(rows >= 0 && cols >= 0);
            }

            /*! destructor
             */
            virtual ~BoostMatrixImpl() = default;

            /*! \returns a matrix of given dimensions, with random elements
                \param[in] rows number of rows to allocate
                \param[in] cols number of columns to allocate
             */
            static std::unique_ptr<IMatrix> rand(int rows, int cols)
            {
                // random
                std::default_random_engine generator;
                std::uniform_real_distribution<double> distribution(0,1);

                // matrix
                auto m = std::unique_ptr<IMatrix>(new BoostMatrixImpl(rows,cols));
                for(int i=0; i<rows; i++)
                {
                    for(int j=0; j<cols; j++)
                    {
                        auto d = distribution(generator);
                        m->set(i,j,d);
                    }
                }
                return std::move(m);
            }

            // --- IMatrix ---
            virtual int rows() const override
            {
                return m_nofRows;
            }

            // --- IMatrix ---
            virtual int cols() const override
            {
                return m_nofCols;
            }

            // --- IMatrix ---
            virtual std::unique_ptr<IMatrix> transpose() const override
            {
                auto matrixPtr = std::unique_ptr<BoostMatrixImpl>(new BoostMatrixImpl(cols(),rows()));
                assert(matrixPtr.get());
                matrixPtr.get()->m_data = boost::numeric::ublas::trans(m_data);
                return std::move(matrixPtr);
            }

            // --- IMatrix ---
            virtual double get(int row, int col) const override
            {
                assert(row >= 0 && row < rows() && col >= 0 && col < cols());
                return m_data(row, col);
            }

            // --- IMatrix ---
            virtual void set(int row, int col, double val) override
            {
                assert(row >= 0 && row < rows() && col >= 0 && col < cols());
                m_data(row, col) = val;
            }

            // --- IMatrix ---
            virtual std::unique_ptr<IMatrix> mul(const IMatrix &m) const override
            {
                auto mPtr = static_cast<const BoostMatrixImpl*>(&m);
                assert(mPtr != nullptr);
                assert(cols() == m.rows());
                auto matrixPtr = new BoostMatrixImpl(m.rows(), m.cols());
                matrixPtr->m_data = boost::numeric::ublas::prod(m_data, mPtr->m_data);
                return std::unique_ptr<IMatrix>(matrixPtr);
            }

        private:
            int                                     m_nofRows;
            int                                     m_nofCols;
            boost::numeric::ublas::matrix<double>   m_data;

    };
}
#endif // BOOST_MATRIX_IMPL_HPP