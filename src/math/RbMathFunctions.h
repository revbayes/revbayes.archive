//
//  RbMathFunctions.h
//  revbayes_xcode
//
//  Created by Sebastian Hoehna on 3/17/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "MatrixReal.h"
#include "VectorReal.h"
#include <vector>

namespace RbMath {
    
    double                      beta(double a, double b);                                                         //!< Xxx 
    double                      factorial(int x);                                                                 //!< Calculate the factorial 
    double                      gamma(double x);                                                                  //!< Calculate the Gamma function 
    double                      incompleteBeta(double a, double b, double x);                                     //!< Xxx 
    double                      incompleteGamma(double x, double alpha, double scale);                            //!< Xxx 
    double                      lnGamma(double a);                                                                //!< Calculate the log of the Gamma function 
    double                      lnFactorial(int n);                                                               //!< Calculate the log factorial 
    double                      rbEpsilon(void);                                                                  //!< Xxx 
}
