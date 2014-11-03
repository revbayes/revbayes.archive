#ifndef Func_averageRateOnBranch_H
#define Func_averageRateOnBranch_H

#include "RlTypedFunction.h"
#include "RealPos.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_averageRateOnBranch : public TypedFunction<RealPos> {
        
    public:
        Func_averageRateOnBranch( void );
        
        // Basic utility functions
        Func_averageRateOnBranch*                       clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction<double>*            createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}

#endif
