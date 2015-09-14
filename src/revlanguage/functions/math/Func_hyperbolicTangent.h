#ifndef Func_hyperbolicTangent_H
#define Func_hyperbolicTangent_H

#include "Probability.h"
#include "RlTypedFunction.h"

#include <string>

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the hyperbolic tangent function.
     *
     * The RevLanguage wrapper of the hyperbolic function function connects
     * the variables/parameters of the function and creates the internal HyperbolicTangentFunction object.
     * Please read the HyperbolicTangentFunction.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-01-31, version 1.0
     *
     */
    class Func_hyperbolicTangent :  public TypedFunction<Probability> {
        
    public:
        Func_hyperbolicTangent( void );
        
        // Basic utility functions
        Func_hyperbolicTangent*                         clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction<double>*            createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}

#endif
