/**
 * @file RbMathVector
 * This file contains the math on vectors.
 *
 * @brief Implementation of vector algebra.
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
#include "VectorReal.h"
#include <vector>

namespace RbMath {
    
    void                        normalize(std::vector<double>& x, double sum);                                    //!< Normalize a vector so that its sum is sum
    void                        vectorMultiplication(const VectorReal& v1, const VectorReal& v2, MatrixReal& p);  //!< Vector multiplication
}
