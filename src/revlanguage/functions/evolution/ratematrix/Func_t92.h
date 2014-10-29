/* 
 * File:   Func_t92.h
 * Author: nl
 *
 * Created on 25 juillet 2014, 20:12
 */

/**
 * @file
 * This file contains the declaration of the RevLanguage HKY function, which
 * is used to created deterministic variable associated with the HKY function.
 *
 * @brief Declaration and implementation of Func_t92
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func__add.h 1406 2012-04-20 02:06:14Z hoehna $
 */

#ifndef FUNC_T92_H
#define	FUNC_T92_H


#include "RlRateMatrix.h"
#include "RlTypedFunction.h"

#include <string>

namespace RevLanguage {
    
    class Func_t92 : public TypedFunction<RateMatrix> {
        
    public:
        Func_t92( void );
        
        // Basic utility functions
        Func_t92*                                                       clone(void) const;                                                              //!< Clone the object
        static const std::string&                                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
    
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::RateMatrix>*         createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                                            getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}

#endif	/* FUNC_T92_H */

