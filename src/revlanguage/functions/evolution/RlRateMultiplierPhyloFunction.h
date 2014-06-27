#ifndef RlRateMultiplierPhyloFunction_H
#define RlRateMultiplierPhyloFunction_H

#include "RlFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class RateMultiplierPhyloFunction :  public Function {
        
    public:
        RateMultiplierPhyloFunction( void );
        
        // Basic utility functions
        RateMultiplierPhyloFunction*                    clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevObject*                               execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
        
    };
    
}

#endif
