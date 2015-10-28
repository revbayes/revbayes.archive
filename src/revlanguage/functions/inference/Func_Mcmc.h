#ifndef Func_Mcmc_H
#define Func_Mcmc_H

#include "RlMonteCarloAnalysis.h"
#include "Procedure.h"

#include <string>

namespace RevLanguage {
    
    /**
     * The RevLanguage contructor function of the Mcmc analysis object.
     *
     * The RevLanguage wrapper of the Mcmc analysis creates the internal Mcmc object.
     * Please read the Mcmc.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-01-12, version 1.0
     *
     */
    class Func_Mcmc : public Procedure {
        
    public:
        Func_Mcmc( void );
        
        // Basic utility functions
        Func_Mcmc*                          clone(void) const;                          //!< Clone the object
        static const std::string&           getClassType(void);                         //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                     //!< Get class type spec
        const std::string&                  getFunctionName(void) const;                //!< Get the primary name of the function in Rev
        const TypeSpec&                     getTypeSpec(void) const;                    //!< Get the type spec of the instance

        // Regular functions
        RevPtr<RevVariable>                 execute(void);                              //!< Execute function
        const ArgumentRules&                getArgumentRules(void) const;               //!< Get argument rules
        const TypeSpec&                     getReturnType(void) const;                  //!< Get type of return value

        
    };
    
}

#endif
