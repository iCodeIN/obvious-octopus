#ifndef IMATRIX_HPP
#define IMATRIX_HPP

#include <assert.h>
#include <functional>
#include <memory>

namespace matrix
{
    /*! In mathematics, a matrix (plural matrices) is a rectangular array of numbers, symbols, or expressions, arranged in rows and columns.
        The individual items in a matrix are called its elements or entries.
        Provided that they are the same size (have the same number of rows and the same number of columns),
        two matrices can be added or subtracted element by element.
        The rule for matrix multiplication, however, is that two matrices can be multiplied only when the number of columns in the first equals the number of rows in the second.
        Any matrix can be multiplied element-wise by a scalar from its associated field.
        A major application of matrices is to represent linear transformations, that is, generalizations of linear functions.
     */
    class IMatrix
    {
        public:
            /*! default constructor
             */
            explicit IMatrix() = default;

            /*! default destructor
             */
            virtual ~IMatrix() = default;

            /*! \return the number of rows
             */
            virtual int rows() const = 0;

            /*! \return the number of cols
             */
            virtual int cols() const = 0;

            /*! \return the transposed matrix
             */
            virtual std::unique_ptr<IMatrix> transpose() const = 0;

            /*! \return the IMatrix that is the result of multiplying this IMatrix with the given IMatrix
             */
            virtual std::unique_ptr<IMatrix> mul(const IMatrix &m) const = 0;

            /*! \return the value at position [row, col]
             */
            virtual double get(int row, int col) const = 0;

            /*! set the value at position [row, col]
             */
            virtual void set(int row, int col, double val) = 0;

            /*! apply an operator to all values of this IMatrix
             */
            virtual void apply(const std::function<double(int row, int col)>& op)
            {
                for(int i=0; i<rows(); i++)
                {
                    for(int j=0; j<cols(); j++)
                    {
                        set(i, j, op(i,j));
                    }
                }
            }

            /*! swap two rows in this IMatrix
             */
            virtual void swapRows(int r0, int r1)
            {
                assert(r0 >= 0 && r0 < rows() && r1 >= 0 && r1 < rows());
                if(r0 == r1)
                {
                    return;
                }
                double temp;
                for(int i = 0 ; i < cols() ; i++)
                {
                    temp = get(r0, i);
                    set(r0, i, get(r1, i));
                    set(r1, i, temp);
                }
            }

            /*! swap two columns in this IMatrix
             */
            virtual void swapCols(int c0, int c1)
            {
                assert(c0 >= 0 && c0 < cols() && c1 >= 0 && c1 < cols());
                if(c0 == c1)
                {
                    return;
                }
                double temp;
                for(int i = 0 ; i < rows() ; i++)
                {
                    temp = get(i, c0);
                    set(i, c0, get(i, c1));
                    set(i, c1, temp);
                }
            }

            /*! \return the smallest value in this IMatrix
             */
            virtual double min() const
            {
                assert(rows() > 0 && cols() > 0);
                auto retval = get(0,0);
                for(int i=0; i<rows(); i++)
                {
                    for(int j=0; j<cols(); j++)
                    {
                        retval = get(i,j) < retval ? get(i,j) : retval;
                    }
                }
                return retval;
            }

            /*! \return the largest value in this IMatrix
             */
            virtual double max()
            {
                assert(rows() > 0 && cols() > 0);
                auto retval = get(0,0);
                for(int i=0; i<rows(); i++)
                {
                    for(int j=0; j<cols(); j++)
                    {
                        retval = get(i,j) > retval ? get(i,j) : retval;
                    }
                }
                return retval;
            }
    };
}

#endif // IMATRIX_HPP