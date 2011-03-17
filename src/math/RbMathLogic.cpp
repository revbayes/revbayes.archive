//
//  RbMathLogic.cpp
//  revbayes_xcode
//
//  Created by Sebastian Hoehna on 3/17/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#include <cassert>
#include <cmath>

#include "RbException.h"
#include "RbMath.h"
#include "RbMathLogic.h"
#include "RbSettings.h"



// Math Logic Functions


/*!
 * The following functions check whether two double-precision real
 * numbers are equal. They are described in:
 *
 * Knuth, D. E. 1981. The art of computer programming: Seminumerical
 *    algorithms, Volume 2. Addison-Wesley.
 *
 * Note that approximately equal to is more stringent than essentially
 * equal to.
 *
 * \brief Comparison functions
 * \param a First double-precision number
 * \param b Second double-precision number
 * \param epsilon How close should the numbers be
 * \return true / false
 */
bool RbMath::compApproximatelyEqual(double a, double b) {
    
    double epsilon = RbSettings::userSettings().getTolerance();
    return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool RbMath::compApproximatelyEqual(double a, double b, double epsilon) {
    
    return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool RbMath::compEssentiallyEqual(double a, double b) {
    
    double epsilon = RbSettings::userSettings().getTolerance();
    return fabs(a - b) <= ( (fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool RbMath::compEssentiallyEqual(double a, double b, double epsilon) {
    
    return fabs(a - b) <= ( (fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool RbMath::compDefinitelyGreaterThan(double a, double b) {
    
    double epsilon = RbSettings::userSettings().getTolerance();
    return (a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool RbMath::compDefinitelyGreaterThan(double a, double b, double epsilon) {
    
    return (a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool RbMath::compDefinitelyLessThan(double a, double b) {
    
    double epsilon = RbSettings::userSettings().getTolerance();
    return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool RbMath::compDefinitelyLessThan(double a, double b, double epsilon) {
    
    return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

