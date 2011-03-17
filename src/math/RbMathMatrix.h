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



#include "MatrixReal.h"
#include "RbMath.h"

namespace RbMath {
    
    void                        computeLandU(MatrixReal& aMat, MatrixReal& lMat, MatrixReal& uMat);               //!< Xxx 
    int                         expMatrixPade(MatrixReal& A, MatrixReal& F, int qValue);                          //!< Xxx 
    int                         findPadeQValue(const double& tolerance);                                          //!< Xxx 
    void                        matrixInverse(MatrixReal& a, MatrixReal& aInv);                                   //!< Compute the inverse of a matrix
    int                         transposeMatrix(const MatrixReal& a, MatrixReal& t);                              //!< Transpose a matrix 
}