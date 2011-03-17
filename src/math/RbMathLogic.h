//
//  RbMathLogic.h
//  revbayes_xcode
//
//  Created by Sebastian Hoehna on 3/17/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "MatrixReal.h"
#include "VectorReal.h"
#include <vector>

namespace RbMath {
    
    bool                        compApproximatelyEqual(double a, double b);                                       //!< Compares two doubles for equality
    bool                        compApproximatelyEqual(double a, double b, double epsilon);                       //!< Compares two doubles for equality
    bool                        compEssentiallyEqual(double a, double b);                                         //!< Compares two doubles for equality
    bool                        compEssentiallyEqual(double a, double b, double epsilon);                         //!< Compares two doubles for equality
    bool                        compDefinitelyGreaterThan(double a, double b);                                    //!< Tests whether one number is greater than another
    bool                        compDefinitelyGreaterThan(double a, double b, double epsilon);                    //!< Tests whether one number is greater than another
    bool                        compDefinitelyLessThan(double a, double b);                                       //!< Tests whether one number is less than another
    bool                        compDefinitelyLessThan(double a, double b, double epsilon);                       //!< Tests whether one number is less than another
}
