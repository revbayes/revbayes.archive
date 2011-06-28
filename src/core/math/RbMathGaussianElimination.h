/**
 * @file RbMathGaussianElimination
 * This file contains the gaussian elimination algorithm to solve linear equation systems.
 *
 * @brief Implementation of the gaussian elimination algorithm.
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

#include "MatrixReal.h"
#include <vector>

namespace RbMath {
    
    void                        backSubstitutionRow(MatrixReal& u, std::vector<double>& b);                       //!< Back substitution for Gaussian elimination 
    void                        forwardSubstitutionRow(MatrixReal& L, std::vector<double>& b);                    //!< Forward substitution for Gaussian elimination 
    void                        gaussianElimination(MatrixReal& a, MatrixReal& bMat, MatrixReal& xMat);           //!< Perform Gaussian elimination on the matrix 
}
