//
//  SimpsonsRule.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "SimpsonsRule.h"
#include "RbException.h"
#include "RbMathLogic.h"
#include "RbSettings.h"

#include <cmath>

using namespace RevBayesCore;

/**
 * We use Simpson's rule for numerical integration.
 * The interval is split into 4 equals parts between a and b.
 * Using Simpson's rule we compute the integral of the left half
 * and the right half of the interval. The approximations of each integral
 * are checked by a recursive call to compute the integral for each half again.
 */
double SimpsonsRule::integrate(const UnivariateFunction &f, double a, double b) const {
    // compute the 4 interval borders
    double midpoint         = (a+b) / 2;
    double midpoint_left    = (a+midpoint) / 2;
    double midpoint_right   = (midpoint+b) / 2;
    
    // compute the function values at each interval endpoint
    double f_a          = f.evaluate( a );
    double f_left_m     = f.evaluate( midpoint_left );
    double f_m          = f.evaluate( midpoint );
    double f_right_m    = f.evaluate( midpoint_right );
    double f_b          = f.evaluate( b );
    
    if ( !RbMath::isFinite(f_a) || !RbMath::isFinite(f_left_m) || !RbMath::isFinite(f_m) || !RbMath::isFinite(f_right_m) || !RbMath::isFinite(f_b) ) {
        throw RbException("Infinite function value.");
    }
    
    double integral_left    = (midpoint-a) / 6 * ( f_a + 4*f_left_m + f_m );
    double integral_right   = (b-midpoint) / 6 * ( f_m + 4*f_right_m + f_b );
    
    double integral = integrateInterval( f, a, midpoint_left, midpoint, f_a, f_left_m, f_m, integral_left) + integrateInterval( f, midpoint, midpoint_right, b, f_m, f_right_m, f_b, integral_right);

    return integral;
}


double SimpsonsRule::integrateInterval(const UnivariateFunction &f, double a, double m, double b, double f_a, double f_m, double f_b, double rough_integral) const {
    // compute the 4 interval borders
    double midpoint_left    = (a+m) / 2;
    double midpoint_right   = (m+b) / 2;
    
    // compute the function values at each interval endpoint
    double f_left_m     = f.evaluate( midpoint_left );
    double f_right_m    = f.evaluate( midpoint_right );
    
    if ( !RbMath::isFinite(f_left_m)  || !RbMath::isFinite(f_right_m) ) {
        throw RbException("Infinite function value.");
    }
    
    double integral_left    = (m-a) / 6 * ( f_a + 4*f_left_m + f_m );
    double integral_right   = (b-m) / 6 * ( f_m + 4*f_right_m + f_b );
    
    double integral = integral_left + integral_right;
    if ( fabs( integral - rough_integral ) > 1E-6 ) {
        integral = integrateInterval( f, a, midpoint_left, m, f_a, f_left_m, f_m, integral_left) + integrateInterval( f, m, midpoint_right, b, f_m, f_right_m, f_b, integral_right);
    }
    
    
    return integral;
}
