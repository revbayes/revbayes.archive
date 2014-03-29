/**
 * @file
 * This file contains the implementation of RateMatrix, which is
 * class that holds a rate matrix in RevBayes.
 *
 * @brief Implementation of RateMatrix
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package distributions
 *
 * $Id$
 */

#include "MatrixReal.h"
#include "RateMatrix.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

void RateMatrix::updateMatrix(void)
{
    // do nothing, though RateMatrixValue::updateMatrix() will implement this
    ;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateMatrix& x) {
    
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}

