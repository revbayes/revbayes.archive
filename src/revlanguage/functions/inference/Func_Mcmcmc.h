#ifndef Func_Mcmcmc_H
#define Func_Mcmcmc_H

#include "RlMonteCarloAnalysis.h"
#include "Procedure.h"

#include <string>

namespace RevLanguage {
    
    /**
     * The RevLanguage contructor function of the Mcmcmc analysis object.
     *
     * The RevLanguage wrapper of the Mcmcmc analysis creates the internal Mcmcmc object.
     * Please read the Mcmcmc.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-01-12, version 1.0
     *
     */
    class Func_Mcmcmc : public Procedure {
        
    public:
        Func_Mcmcmc( void );
        
        // Basic utility functions
        Func_Mcmcmc*                        clone(void) const;                          //!< Clone the object
        static const std::string&           getClassType(void);                         //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                     //!< Get class type spec
        std::string                         getFunctionName(void) const;                //!< Get the primary name of the function in Rev
        const TypeSpec&                     getTypeSpec(void) const;                    //!< Get the type spec of the instance
        
        // Regular functions
        RevPtr<RevVariable>                 execute(void);                              //!< Execute function
        const ArgumentRules&                getArgumentRules(void) const;               //!< Get argument rules
        const TypeSpec&                     getReturnType(void) const;                  //!< Get type of return value
        
        
    };
    
}

#endif
