//
//  TrapezoidalRule.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TrapezoidalRule.h"
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
double TrapezoidalRule::integrate(const UnivariateFunction &f, double a, double b) const {
    
    if (a == b) {
        return 0.0;
    }
    
    // compute the 4 interval borders
    double midpoint         = (a+b) / 2;
    
    // compute the function values at each interval endpoint
    double f_a          = f.evaluate( a );
    double f_m          = f.evaluate( midpoint );
    double f_b          = f.evaluate( b );
    
    if ( !RbMath::isFinite(f_a) || !RbMath::isFinite(f_m) || !RbMath::isFinite(f_b) ) {
        throw RbException("Infinite function value.");
    }
    
    double integral_left    = (midpoint-a) / 2.0 * ( f_a + f_m );
    double integral_right   = (b-midpoint) / 2.0 * ( f_m + f_b );
    
    double integral = integrateInterval( f, a, midpoint, f_a, f_m, integral_left) + integrateInterval( f, midpoint, b, f_m, f_b, integral_right);
    
    return integral;
}


double TrapezoidalRule::integrateInterval(const UnivariateFunction &f, double a, double b, double f_a, double f_b, double rough_integral) const {
    
    if (a == b) {
        return 0.0;
    }
    
    // compute the 4 interval borders
    double m = (a+b) / 2.0;
    
    // compute the function values at each interval endpoint
    double f_m = f.evaluate( m );
    
    if ( !RbMath::isFinite(f_m) ) {
        throw RbException("Infinite function value.");
    }
    
    double integral_left    = (m-a) / 2.0 * ( f_a + f_m );
    double integral_right   = (b-m) / 2.0 * ( f_m + f_b );
    
    double integral = integral_left + integral_right;
    if ( fabs( rough_integral - integral ) > 1E-5 && fabs( 1.0 - rough_integral / integral ) > 1E-5 && fabs(a-b) > 1E-4 ) {
        integral = integrateInterval( f, a, m, f_a, f_m, integral_left) + integrateInterval( f, m, b, f_m, f_b, integral_right);
    }
    
    
    return integral;
}
