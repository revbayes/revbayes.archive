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

#include "Complex.h"
#include "Matrix.h"
#include "Real.h"
#include "RbVector.h"



namespace RbMath {

    void                        computeLandU(Matrix<Real>& aMat, Matrix<Real>& lMat, Matrix<Real>& uMat);               //!< Compute L & U decomposition of a matrix
    void                        computeLandU(Matrix<Complex>& aMat, Matrix<Complex>& lMat, Matrix<Complex>& uMat);      //!< Compute L & U decomposition of a matrix
    int                         expMatrixPade(Matrix<Real>& A, Matrix<Real>& F, int qValue);                            //!< Exponentiate the matrix A using the Pade method
    int                         findPadeQValue(const double& tolerance);                                                //!< Find the pq values for controlling the tolerance of the Pade method
    void                        matrixInverse(const Matrix<Complex>& a, Matrix<Complex>& aInv);                         //!< Compute the inverse of a matrix
    void                        matrixInverse(const Matrix<Real>& a, Matrix<Real>& aInv);                               //!< Compute the inverse of a matrix
    int                         transposeMatrix(const Matrix<Real>& a, Matrix<Real>& t);                                //!< Transpose a matrix

    RbVector<Real>              colSumMatrix(const Matrix<Real>& a);
    RbVector<Real>              rowSumMatrix(const Matrix<Real>& a);
    RbVector<Real>              colMeansMatrix(const Matrix<Real>& a);
    RbVector<Real>              rowMeansMatrix(const Matrix<Real>& a);
}
