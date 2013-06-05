/**
 * @file
 * This file contains the declaration of BurninEstimationConvergenceAssessmentFunction, which is
 * the function used estimate the optimal burnin on the criteria:
 *    ESS: the burnin is choosen so that the ESS is maximized
 *    SEM: the burnin is choosen so that the SEM is minimized
 * and assess convergence based on:
 *    Geweke-statistic: compares a first part of the chain to a second part
 *    
 *
 * @brief Declaration of OptimalBurninFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-22 12:06:58 -0700 (Sun, 22 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2013-04-04
 *
 * $Id: Func_source.h 1431 2012-04-22 19:06:58Z hoehna $
 */

#ifndef BurninEstimationConvergenceAssessmentFunction_H
#define BurninEstimationConvergenceAssessmentFunction_H

#include "RlFunction.h"

namespace RevLanguage {
    
    class BurninEstimationConvergenceAssessmentFunction : public Function {
        
    public:
        BurninEstimationConvergenceAssessmentFunction( void );
        
        // Basic utility functions
        BurninEstimationConvergenceAssessmentFunction*      clone(void) const;                                                          //!< Clone object
        static const std::string&                           getClassName(void);                                                         //!< Get class name
        static const TypeSpec&                              getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                                     getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_source functions
        const ArgumentRules&                                getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                                     getReturnType(void) const;                                                  //!< Get type of return val
        bool                                                throws(void) const { return false; }                                         //!< Function may throw exceptions
        
        RbLanguageObject*                                   execute(void);                                                              //!< Execute function
        
    };
    
}

#endif

