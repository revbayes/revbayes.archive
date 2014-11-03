/**
 * @file
 * This file contains the declaration of Func_discretizeGamma,
 * which is used to normalize vectors, creating a
 * simplex.
 *
 * @brief Declaration of Func_discretizeGamma
 *
 * (c) Copyright 2009- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 */

#ifndef Func_discretizeGamma_H
#define Func_discretizeGamma_H

#include "ModelVector.h"
#include "RealPos.h"
#include "RlTypedFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_discretizeGamma :  public TypedFunction< ModelVector< RealPos > > {
        
    public:
        Func_discretizeGamma();
        
        // Basic utility functions
        Func_discretizeGamma*                                               clone(void) const;                                          //!< Clone the object
        static const std::string&                                           getClassName(void);                                         //!< Get class name
        static const TypeSpec&                                              getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&                                                     getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        RevBayesCore::TypedFunction< RevBayesCore::RbVector<double> >*      createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                                                getArgumentRules(void) const;                               //!< Get argument rules
        
    };
    
}


#endif


