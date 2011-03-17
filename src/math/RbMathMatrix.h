//
//  RbMathMatrix.h
//  revbayes_xcode
//
//  Created by Sebastian Hoehna on 3/17/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#include "MatrixReal.h"
#include "RbMath.h"

namespace RbMath {
    
    void                        computeLandU(MatrixReal& aMat, MatrixReal& lMat, MatrixReal& uMat);               //!< Xxx 
    int                         expMatrixPade(MatrixReal& A, MatrixReal& F, int qValue);                          //!< Xxx 
    int                         findPadeQValue(const double& tolerance);                                          //!< Xxx 
    void                        matrixInverse(MatrixReal& a, MatrixReal& aInv);                                   //!< Compute the inverse of a matrix
    int                         transposeMatrix(const MatrixReal& a, MatrixReal& t);                              //!< Transpose a matrix 
}