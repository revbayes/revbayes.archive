#ifndef Func__mod_H
#define Func__mod_H

#include "RlFunction.h"
#include <string>

namespace RevLanguage {
    
    
    /**
     * @brief Modulo function.
     *
     * Performs the modulo operation of two natural numbers, e.g.,
     * 7 % 5 = 2
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2014-07-04
     *
     */
    class Func__mod: public Function {
        
    public:
        Func__mod();
        
        // Basic utility functions
        Func__mod*                  clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
        
        
        RevPtr<Variable>            execute(void);                                              //!< Execute function
        
    };
    
}

#endif


