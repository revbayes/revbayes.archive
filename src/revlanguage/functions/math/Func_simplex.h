#ifndef Func_simplex_H
#define Func_simplex_H

#include "RlSimplex.h"
#include "RlTypedFunction.h"

#include <string>

namespace RevLanguage {
    
    /**
     * @brief Simplex constructor from RealPos values
     *
     * This function constructs a simplex from two or more RealPos values.
     */
    class Func_simplex : public TypedFunction<Simplex> {
        
    public:
        Func_simplex();
        
        // Basic utility functions
        Func_simplex*                                                               clone(void) const;                                          //!< Clone the object
        static const std::string&                                                   getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&                                                      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&                                                             getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        RevBayesCore::TypedFunction< RevBayesCore::RbVector< double > >*            createFunction(void) const;                                 //!< Create internal function object
        const ArgumentRules&                                                        getArgumentRules(void) const;                               //!< Get argument rules
        
    };
    
}


#endif


