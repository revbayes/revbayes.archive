/**
 * @file RbMathCombinatorialFunctions
 * This file contains some useful (standard) combinatorial math functions.
 *
 * @brief Implementation of standard combinatorial math functions.
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


#ifndef RbMathCombinatorialFunctions_H
#define RbMathCombinatorialFunctions_H

#include <vector>

namespace RevBayesCore {

    namespace RbMath {

        double                      choose(double n, double k);                                                     //!< Calculate the factorial
        int                         kchoose2(int k);                                                                //!< Number of ways to choose 2 elements from k
        double                      factorial(int x);                                                               //!< Calculate the factorial
        double                      lnChoose(double n, double k);                                                   //!< Calculate the log factorial
        double                      lnFactorial(int n);                                                             //!< Calculate the log factorial
		int							stirlingFirst(int n, int k);											//!< Calculate the log Stirling number of the first kind

        // helper functions
        double                      lfastchoose(double n, double k);                                                //!< Calculate the log factorial
        double                      lfastchoose2(double n, double k, int* s_choose);                                //!< Calculate the log factorial

    }
    
}

#endif
