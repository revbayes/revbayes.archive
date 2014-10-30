/**
 * @file
 * This file contains the declaration of the RevLanguage TreeHeight statistic, which
 * is used to created deterministic variable associated with the TreeHeight statistic.
 *
 * @brief Declaration and implementation of TreeheightStatistic
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func__add.h 1406 2012-04-20 02:06:14Z hoehna $
 */


#ifndef Func_treeHeight_H
#define Func_treeHeight_H

#include "RlTypedFunction.h"
#include "RealPos.h"

#include <string>

namespace RevLanguage {
    
    class Func_treeHeight :  public TypedFunction<RealPos> {
        
    public:
        Func_treeHeight( void );
        
        // Basic utility functions
        Func_treeHeight*                                clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction<double>*            createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}

#endif
