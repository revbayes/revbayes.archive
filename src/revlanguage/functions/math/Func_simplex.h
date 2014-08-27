#ifndef Func_simplex_H
#define Func_simplex_H

#include "RlFunction.h"
#include <map>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Simplex constructor from RealPos values
     *
     * This function constructs a simplex from two or more RealPos values.
     */
    class Func_simplex :  public Function {
        
    public:
        Func_simplex();
        
        // Basic utility functions
        Func_simplex*               clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
        
        
        RevPtr<Variable>            execute(void);                                              //!< Execute function
        
    };
    
}


#endif


