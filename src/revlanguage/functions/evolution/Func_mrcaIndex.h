/**
 * @file
 * This file contains the declaration of the RevLanguage mrca function, which
 * is used to give the node or node index for the mrca of a Clade.
 *
 * @brief Declaration and implementation of Func_mrcaIndex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 */


#ifndef RlMrcaStatistic_H
#define RlMrcaStatistic_H

#include "RlFunction.h"

#include <string>

namespace RevLanguage {
    
    class Func_mrcaIndex :  public Function {
        
    public:
        Func_mrcaIndex( void );
        
        // Basic utility functions
        Func_mrcaIndex*                                      clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevPtr<Variable>                                execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
        
    };
    
}

#endif
