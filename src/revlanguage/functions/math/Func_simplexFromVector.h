#ifndef Func_simplexFromVector_H
#define Func_simplexFromVector_H

#include "RlFunction.h"
#include <string>

namespace RevLanguage {
    
    class Func_simplexFromVector :  public Function {
        
    public:
        Func_simplexFromVector();
        
        // Basic utility functions
        Func_simplexFromVector*     clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassType(void);                                         //!< Get Rev type (static)
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get Rev type spec (static)
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get Rev type spec (from instance)
        
        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type spec of return value
        
        
        RevPtr<Variable>            execute(void);                                              //!< Execute function
        
    };
    
}


#endif


