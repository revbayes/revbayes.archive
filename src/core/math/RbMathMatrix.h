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
 * $Id:$
 */

#include "MatrixComplex.h"
#include "MatrixReal.h"



namespace RbMath {
    
    void                        computeLandU(MatrixReal& aMat, MatrixReal& lMat, MatrixReal& uMat);               //!< Compute L & U decomposition of a matrix 
    void                        computeLandU(MatrixComplex& aMat, MatrixComplex& lMat, MatrixComplex& uMat);      //!< Compute L & U decomposition of a matrix 
    int                         expMatrixPade(MatrixReal& A, MatrixReal& F, int qValue);                          //!< Exponentiate the matrix A using the Pade method 
    int                         findPadeQValue(const double& tolerance);                                          //!< Find the pq values for controlling the tolerance of the Pade method 
    void                        matrixInverse(const MatrixComplex& a, MatrixComplex& aInv);                       //!< Compute the inverse of a matrix
    void                        matrixInverse(const MatrixReal& a, MatrixReal& aInv);                             //!< Compute the inverse of a matrix
    int                         transposeMatrix(const MatrixReal& a, MatrixReal& t);                              //!< Transpose a matrix 
}