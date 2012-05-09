/**
 * @file RbMathMatrix
 * This file contains the math on matrices.
 *
 * @brief Implementation of matrix algebra.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-03-17, version 1.0
 *
 * $Id$
 */

#include "Matrix.h"

#include <complex>


namespace RbMath {

    void                        computeLandU(Matrix<double>& aMat, Matrix<double>& lMat, Matrix<double>& uMat);                                                     //!< Compute L & U decomposition of a matrix
    void                        computeLandU(Matrix<std::complex<double> >& aMat, Matrix<std::complex<double> >& lMat, Matrix<std::complex<double> >& uMat);        //!< Compute L & U decomposition of a matrix
    int                         expMatrixPade(Matrix<double>& A, Matrix<double>& F, int qValue);                                                                    //!< Exponentiate the matrix A using the Pade method
    int                         findPadeQValue(const double& tolerance);                                                                                            //!< Find the pq values for controlling the tolerance of the Pade method
    void                        matrixInverse(const Matrix<std::complex<double> >& a, Matrix<std::complex<double> >& aInv);                                         //!< Compute the inverse of a matrix
    void                        matrixInverse(const Matrix<double>& a, Matrix<double>& aInv);                                                                       //!< Compute the inverse of a matrix
    int                         transposeMatrix(const Matrix<double>& a, Matrix<double>& t);                                                                        //!< Transpose a matrix

    std::vector<double>         colSumMatrix(const Matrix<double>& a);
    std::vector<double>         rowSumMatrix(const Matrix<double>& a);
    std::vector<double>         colMeansMatrix(const Matrix<double>& a);
    std::vector<double>         rowMeansMatrix(const Matrix<double>& a);
}
