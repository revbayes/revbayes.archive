/**
 * @file
 * This file contains the declaration of Func_beca, which is
 * the function used estimate the optimal burnin on the criteria:
 *    ESS: the burnin is choosen so that the ESS is maximized
 *    SEM: the burnin is choosen so that the SEM is minimized
 * and assess convergence based on:
 *    Geweke-statistic: compares a first part of the chain to a second part
 *    
 *
 * @brief Declaration of Func_beca
 *
 * (c) Copyright 2009- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 */

#ifndef Func_beca_H
#define Func_beca_H

#include "RlFunction.h"

namespace RevLanguage {
    
    class Func_beca : public Function {
        
    public:
        Func_beca( void );
        
        // Basic utility functions
        Func_beca*                                          clone(void) const;                                                          //!< Clone object
        static const std::string&                           getClassName(void);                                                         //!< Get class name
        static const TypeSpec&                              getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                                     getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_source functions
        const ArgumentRules&                                getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                                     getReturnType(void) const;                                                  //!< Get type of return val
        bool                                                throws(void) const { return false; }                                        //!< Function may throw exceptions
        
        RevObject*                                          execute(void);                                                              //!< Execute function
        
    };
    
}

#endif

