#ifndef Func_hiddenStateRateMatrix_H
#define Func_hiddenStateRateMatrix_H

#include "RlTypedFunction.h"
#include "RlRateMatrix.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the Free-K rate matrix function.
     *
     * The RevLanguage wrapper of the Free-K rate matrix connects
     * the variables/parameters of the function and creates the internal hiddenStateRateMatrixFunction object.
     * Please read the hiddenStateRateMatrixFunction.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2016-09-07, version 1.0
     *
     */
    class Func_hiddenStateRateMatrix :  public TypedFunction<RateMatrix> {
        
    public:
        Func_hiddenStateRateMatrix( void );
        
        // Basic utility functions
        Func_hiddenStateRateMatrix*                                   clone(void) const;                                          //!< Clone the object
        static const std::string&                                       getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&                                          getClassTypeSpec(void);                                     //!< Get class type spec
        std::string                                                     getFunctionName(void) const;                                //!< Get the primary name of the function in Rev
        const TypeSpec&                                                 getTypeSpec(void) const;                                    //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >*     createFunction(void) const;                                 //!< Execute function
        const ArgumentRules&                                            getArgumentRules(void) const;                               //!< Get argument rules
        
    };
    
}

#endif
