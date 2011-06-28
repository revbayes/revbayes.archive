/**
 * @file RbMathLogic
 * This file contains some logic function on numbers.
 *
 * @brief Implementation of simple boolean algebra on numbers.
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


#include <cassert>
#include <cmath>

#include "RbException.h"
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

bool RbMath::compDefinitelyGreaterThan(double a, double b, double epsilon) 
{    
    return (a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool RbMath::compDefinitelyLessThan(double a, double b) 
{    
    double epsilon = RbSettings::userSettings().getTolerance();
    return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool RbMath::compDefinitelyLessThan(double a, double b, double epsilon) 
{    
    return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool RbMath::isFinite(double x) {
    
    // TODO: No isfinite function in Microsoft math; isFinite leads to infinite recursion
    // return isFinite(x);
    return false;
}

bool RbMath::isInt(double x)
{
    double int_part;
    return ( modf ( x, &int_part ) == 0.0 );
}

bool RbMath::isNan(double x) {
    return x != x;
}

double RbMath::max(double x, double y)
{
	return (x < y) ? y : x;
}

double RbMath::min(double x, double y) 
{
    return (x < y) ? x : y;
}


