#pragma once
#ifndef MATRIX_GAUSSIANELIMINATION_HPP
#define MATRIX_GAUSSIANELIMINATION_HPP

#include "matrix/imatrix.hpp"

namespace matrix
{
    namespace algorithm
    {
        /*! In linear algebra, Gaussian elimination (also known as row reduction) is an algorithm
            for solving systems of linear equations. It is usually understood as a sequence of
            operations performed on the associated matrix of coefficients. This method can also be
            used to find the rank of a matrix, to calculate the determinant of a matrix, and to
            calculate the inverse of an invertible square matrix. The method is named after
            Carl Friedrich Gauss (1777–1855), although it was known to Chinese mathematicians as
            early as 179 CE.

            Performs row reduction on a matrix, using a sequence of elementary row operations to
        	 modify the matrix until the lower left-hand corner of the matrix is filled with zeros,
        	 as much as possible. There are three types of elementary row operations:
        	 - Swapping two rows,
        	 - Multiplying a row by a non-zero number,
        	 - Adding a multiple of one row to another row.

        	 Using these operations, a matrix can always be transformed into an upper triangular matrix,
        	 and in fact one that is in row echelon form.
         */
        static void gaussianElimination(IMatrix &matrix)
        {
            // Get to upper triangular form
            auto pivot = 0;
            while(pivot != matrix.rows())
            {

                // Find (absolute value) largest pivot
                double maxPivot = matrix.get(pivot, pivot);
                int maxPivotRow = pivot;
                for(int i=pivot; i<matrix.rows(); i++)
                {
                    if(abs(matrix.get(pivot, pivot)) > maxPivot)
                    {
                        maxPivot = abs(matrix.get(i, pivot));
                        maxPivotRow = i;
                    }
                }
                // Swap rows to place pivot in correct position
                matrix.swapRows(pivot, maxPivotRow);

                // Divide each element in the row by the pivot
                double pivotValue = matrix.get(pivot, pivot);
                for(int i=pivot+1; i<matrix.cols(); i++)
                {
                    matrix.set(pivot, i, matrix.get(pivot,i) / pivotValue);
                }
                matrix.set(pivot, pivot         , 1.0);

                // Handle subsequent rows
                for(int i=pivot+1; i<matrix.rows(); i++)
                {
                    double factor = matrix.get(i, pivot);
                    for(int j = pivot + 1 ; j < matrix.cols() ; j++)
                    {
                        matrix.set(i, j, matrix.get(i, j) - matrix.get(pivot, j) * factor);
                    }
                    matrix.set(i, pivot, 0.0);

                }

                // Find next pivot
                pivot++;
            }

            // Backsubstitution
            pivot = matrix.rows() - 1;
            while(pivot > 0)
            {
                // Substitute this row in the rows above it
                for(int i=pivot-1; i>=0; i--)
                {
                    double pivotValue = matrix.get(i, pivot);
                    for(int j=0; j<matrix.cols(); j++)
                    {
                        matrix.set(i, j, matrix.get(i, j) - matrix.get(pivot, j) * pivotValue);
                    }
                }
                // Find next pivot
                pivot--;
            }
        }
    }
}

#endif // MATRIX_GAUSSIAN_ELIMINATION_HPP
