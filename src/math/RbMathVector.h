//
//  RbMathVector.h
//  revbayes_xcode
//
//  Created by Sebastian Hoehna on 3/17/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "MatrixReal.h"
#include "VectorReal.h"
#include <vector>

namespace RbMath {
    
    void                        normalize(std::vector<double>& x, double sum);                                    //!< Normalize a vector so that its sum is sum
    void                        vectorMultiplication(const VectorReal& v1, const VectorReal& v2, MatrixReal& p);  //!< Vector multiplication
}
