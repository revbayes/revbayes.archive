/**
 * @file
 * This file contains the declaration of the
 * Func_powermix class, which is used to compute a
 * power mixture of two simplex parameters.
 *
 * @brief Declaration of Func_powermix
 *
 * (c) Copyright under GPL version 3
 */

#ifndef RevBayes_Func_powermix_h
#define RevBayes_Func_powermix_h

#include "RlSimplex.h"
#include "RlTypedFunction.h"

#include <string>

namespace RevLanguage {
    
    /**
     *   @brief Func_powermix
     *
     *   Function that takes two simplex parameters and mixes them in
     *   proportion to the powers of a two-component simplex.
     */
    class Func_powermix : public TypedFunction<Simplex> {
        
    public:
        Func_powermix( void );
        
        // Basic utility functions
        Func_powermix*                                                              clone(void) const;                                                              //!< Clone the object
        static const std::string&                                                   getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                                                      getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                                             getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::RbVector< double > >*            createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                                                        getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}

#endif
