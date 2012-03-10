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
 * $Id$
 */

#include "Complex.h"
#include "Matrix.h"
#include "Real.h"
#include <vector>



namespace RbMath {
    
    void                        backSubstitutionRow(Matrix<Complex>& u, std::vector<std::complex<double> >& b);     //!< Back substitution for Gaussian elimination 
    void                        backSubstitutionRow(Matrix<Real>& u, std::vector<double>& b);                       //!< Back substitution for Gaussian elimination 
    void                        forwardSubstitutionRow(Matrix<Complex>& L, std::vector<std::complex<double> >& b);  //!< Forward substitution for Gaussian elimination 
    void                        forwardSubstitutionRow(Matrix<Real>& L, std::vector<double>& b);                    //!< Forward substitution for Gaussian elimination 
    void                        gaussianElimination(Matrix<Real>& a, Matrix<Real>& bMat, Matrix<Real>& xMat);       //!< Perform Gaussian elimination on the matrix 
}
