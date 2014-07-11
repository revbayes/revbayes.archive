/**
 * @file This file contains the declaration of the RevLanguage BPP
 * models related function, which is used to created deterministic
 * variable associated with the BPP models.
 *
 * @brief Declaration and implementation of Func_BppGeneticCode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: vendredi 11 juillet 2014, à 11h 40 $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_BppGeneticCode.h 1406 2012-04-20 02:06:14Z gueguen $
 */


#ifndef Func_BppGeneticCode_H
#define Func_BppGeneticCode_H

#include "RlFunction.h"

#include <string>

namespace RevLanguage {
    
    class Func_BppGeneticCode :  public Function {
        
    public:
        Func_BppGeneticCode( void );
        
        // Basic utility functions
      Func_BppGeneticCode*                              clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
    
        // Function functions you have to override
        RevObject*                                      execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
        
    };
    
}

#endif
