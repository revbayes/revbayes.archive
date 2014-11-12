#ifndef Func_phyloRateMultiplier_H
#define Func_phyloRateMultiplier_H

#include "ModelVector.h"
#include "RealPos.h"
#include "RlTypedFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_phyloRateMultiplier : public TypedFunction< ModelVector<RealPos> > {
        
    public:
        Func_phyloRateMultiplier( void );
        
        // Basic utility functions
        Func_phyloRateMultiplier*                                                   clone(void) const;                                                              //!< Clone the object
        static const std::string&                                                   getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                                                      getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                                             getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::RbVector< double > >*            createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                                                        getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}

#endif
