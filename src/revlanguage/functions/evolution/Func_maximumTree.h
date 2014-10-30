/**
 * @file
 * This file contains the declaration of the RevLanguage maximumTree function, which
 * is used to create a deterministic variable associated with the maximum tree computed from a vector of gene trees.
 *
 * @brief Declaration and implementation of maximumTree function
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_maximumTree.h 1406 2012-04-20 02:06:14Z Boussau $
 */


#ifndef Func_maximumTree_H
#define Func_maximumTree_H

#include "RlTimeTree.h"
#include "RlTypedFunction.h"

#include <string>

namespace RevLanguage {
    
    class Func_maximumTree : public TypedFunction<TimeTree> {
        
    public:
        Func_maximumTree( void );
        
        // Basic utility functions
        Func_maximumTree*                                               clone(void) const;                                                              //!< Clone the object
        static const std::string&                                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::TimeTree>*           createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                                            getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}

#endif
