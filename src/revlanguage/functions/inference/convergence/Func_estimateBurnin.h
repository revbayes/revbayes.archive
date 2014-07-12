/**
 * @file
 * This file contains the declaration of Func_estimateBurnin, which is
 * the function used estimate the optimal burnin on the criteria:
 *    ESS: the burnin is choosen so that the ESS is maximized
 *    SEM: the burnin is choosen so that the SEM is minimized
 *
 * @brief Declaration of Func_estimateBurnin
 *
 * (c) Copyright 2009- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 */

#ifndef Func_estimateBurnin_H
#define Func_estimateBurnin_H

#include "RlFunction.h"

namespace RevLanguage {
    
    class Func_estimateBurnin : public Function {
        
    public:
        Func_estimateBurnin( void );
        
        // Basic utility functions
        Func_estimateBurnin*                    clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_source functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                        //!< Function may throw exceptions
        
        RevPtr<Variable>                        execute(void);                                                              //!< Execute function
        
    };
    
}

#endif

