#ifndef Func_covarion_H
#define Func_covarion_H

#include "RlTypedFunction.h"
#include "RlRateMatrix.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the covarion rate matrix function.
     *
     * The RevLanguage wrapper of the covarion Rate Matrix connects
     * the variables/parameters of the function and creates the internal CovarionRateMatrixFunction object.
     * Please read the CovarionRateMatrixFunction.h for more info.
     *
     * @author Sebastian Hoehna & Lyndon Coghill
     *
     */
    class Func_covarion :  public TypedFunction<RateMatrix> {
        
    public:
        Func_covarion( void );
        
        // Basic utility functions
        Func_covarion*                                                  clone(void) const;                                          //!< Clone the objectz
        static const std::string&                                       getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&                                          getClassTypeSpec(void);                                     //!< Get class type spec
        std::string                                                     getFunctionName(void) const;                                //!< Get the primary name of the function in Rev
        const TypeSpec&                                                 getTypeSpec(void) const;                                    //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >*     createFunction(void) const;                                 //!< Execute function
        const ArgumentRules&                                            getArgumentRules(void) const;                               //!< Get argument rules
        
    };
    
}

#endif /* Func_covarion_h */
